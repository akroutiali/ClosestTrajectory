#include "closesttrajectory.hpp"

bool comparePoints(Point a, Point b) {
    return a.t < b.t;
}
Trajectory::Trajectory(const vector<Point>& points) : points_(points) {
    std::sort(points_.begin(),points_.end(),comparePoints);
}
Trajectory::~Trajectory() {}

double Trajectory::distance(Point a, Point b) const{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double Trajectory::length() const {
    double length = 0;
#pragma omp parallel for reduction(+:length)
    for (int i = 1; i < points_.size(); i++) {
        length += distance(points_[i], points_[i-1]);
    }
    return length;
}

double Trajectory::speed() const {
    double total_length = 0;
    double total_time = 0;

#pragma omp parallel for reduction(+:total_length,total_time)
    for (int i = 1; i < points_.size(); i++) {
        total_time += points_[i].t - points_[i-1].t;
        total_length += distance(points_[i], points_[i-1]);
    }
    return total_length / total_time;
}

vector<Point>& Trajectory::get_data(){
    return points_;
}

ClosestTrajectory::ClosestTrajectory(const string& url_name):url_(url_name) {}

ClosestTrajectory::~ClosestTrajectory() {}
double ClosestTrajectory::lengthDifference(const  Trajectory& t1,  const Trajectory& t2) {
    double len1 = t1.length();
    double len2 = t2.length();
    return abs(len1 - len2);
}

double ClosestTrajectory::speedDifference(const Trajectory& t1, const Trajectory& t2) {
    double speed1 = t1.speed();
    double speed2 = t2.speed();
    return abs(speed1 - speed2);
}

vector<Trajectory> ClosestTrajectory::readTrajectories() {
    vector<Trajectory> trajectories;
    try {
        http_client client(url_);
        auto response = client.request(methods::GET).get();

        if (response.status_code() == status_codes::OK) {
            concurrency::streams::ostream outfile = concurrency::streams::file_stream<uint8_t>::open_ostream("data.txt").get();
            response.body().read_to_end(outfile.streambuf()).wait();

            // Read trajectories from the file
            std::ifstream file("data.txt", std::ios::binary);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file");
            }
            int num_trajectories;
            file >> num_trajectories;
            for (int i = 0; i < num_trajectories; i++) {
                int num_points;
                file >> num_points;
                vector<Point> points(num_points);
                for (int j = 0; j < num_points; j++) {
                    file >> points[j].x >> points[j].y >> points[j].t;
                }
                trajectories.emplace_back(points);
            }
        } else {
            std::cout << "Error downloading file. Status code: " << response.status_code() << std::endl;
        }
    }catch (const std::exception& ex) {
        std::cerr << "Exception caught: " << ex.what() << std::endl;
    }
    return trajectories;
}
/*
    -Time Complexity :O(nlogn).
*/
vector<Trajectory> ClosestTrajectory::FindClosestTrajectories( Trajectory& reference,  vector<Trajectory>& trajectories,  string& metric_type) {
    vector<pair<Trajectory, double>> distances;

    for ( auto& t : trajectories) {
        double distance = 0;
        if (metric_type == "length") {
            distance = lengthDifference(reference, t);
        } else if (metric_type == "speed") {
            distance = speedDifference(reference, t);
        }
        distances.emplace_back(t, distance);
    }

    sort(distances.begin(), distances.end(), [](const auto& a, const auto& b) { return a.second < b.second; });

    vector<Trajectory> closest_trajectories;
    for (int i = 0; i < min(3, static_cast<int>(trajectories.size())); i++) {
        closest_trajectories.push_back(distances[i].first);
    }

    return closest_trajectories;
}

/*
    time complexity O(nlogk)

*/

vector<Trajectory> ClosestTrajectory::FindClosestTrajectories_Pq(Trajectory& reference, vector<Trajectory>& trajectories, string& metric_type) {
    auto comparator = [](const auto& a, const auto& b) { return a.second > b.second; };
    priority_queue<pair<Trajectory, double>, vector<pair<Trajectory, double>>, decltype(comparator)> pq(comparator);

    for (auto& t : trajectories) {
        double distance = 0;
        if (metric_type == "length") {
            distance = lengthDifference(reference, t);
        } else if (metric_type == "speed") {
            distance = speedDifference(reference, t);
        }
        pq.emplace(t, distance);
    }

    vector<Trajectory> closest_trajectories;
    for (int i = 0; i < min(3, static_cast<int>(trajectories.size())); i++) {
        closest_trajectories.push_back(pq.top().first);
        pq.pop();
    }

    return closest_trajectories;
}

/*
    Time Complexity : O(nlogn)
*/

vector<Trajectory> ClosestTrajectory::FindClosestTrajectories_Ms(Trajectory& reference, vector<Trajectory>& trajectories, string& metric_type) {
    auto comparator = [](const auto& a, const auto& b) { return a.second < b.second; };
    multiset<pair<Trajectory, double>, decltype(comparator)> ms(comparator);

    for (auto& t : trajectories) {
        double distance = 0;
        if (metric_type == "length") {
            distance = lengthDifference(reference, t);
        } else if (metric_type == "speed") {
            distance = speedDifference(reference, t);
        }
        ms.emplace(t, distance);
    }

    vector<Trajectory> closest_trajectories;
    auto iter = ms.begin();
    for (int i = 0; i < min(3, static_cast<int>(trajectories.size())); i++, iter++) {
        closest_trajectories.push_back(iter->first);
    }

    return closest_trajectories;
}

/*
    Time Complexity : O(logn)
*/
vector<Trajectory> ClosestTrajectory::FindClosestTrajectories_FlannTree(Trajectory& reference, vector<Trajectory>& trajectories, string& metric_type) {
    int num_trajectories = static_cast<int>(trajectories.size());
    int num_dimensions = 3;

    // Allocate memory for the input and output data
    float* input_data = new float[num_trajectories * num_dimensions];
    float* output_data = new float[num_dimensions];

    // Convert the trajectories into a format that FLANN understands
    for (int i = 0; i < num_trajectories; i++) {
        input_data[i * num_dimensions] = trajectories[i].get_data()[0].x;
        input_data[i * num_dimensions + 1] = trajectories[i].get_data()[0].y;
        input_data[i * num_dimensions + 2] = trajectories[i].get_data()[0].t;
    }

    // Build the index for the input data
    flann::Matrix<float> inputData(input_data, num_trajectories, num_dimensions);
    flann::Index<flann::L2_Simple<float>> index(inputData, flann::HierarchicalClusteringIndexParams(16, flann::FLANN_CENTERS_KMEANSPP, 4));
    index.buildIndex();

    // Find the k-nearest neighbors
    int num_neighbors = 3;
    flann::Matrix<int> indices(new int[num_neighbors * num_trajectories], num_trajectories, num_neighbors);
    flann::Matrix<float> dists(new float[num_neighbors * num_trajectories], num_trajectories, num_neighbors);
    flann::Matrix<float> query(output_data, 1, num_dimensions);

    index.knnSearch(query, indices, dists, num_neighbors, flann::SearchParams(128));

    // Extract the k-nearest trajectories
    vector<Trajectory> closest_trajectories;
    for (int i = 0; i < num_neighbors; i++) {
        closest_trajectories.push_back(trajectories[indices[0][i]]);
    }

    // Free memory
    delete[] input_data;
    delete[] output_data;
    delete[] indices.ptr();
    delete[] dists.ptr();

    return closest_trajectories;
}