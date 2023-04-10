#include "closesttrajectory.hpp"


int main(int argc, char* argv[]) {
    if(argc <2) {
        cout<< "No URL for data file   is passed "<<endl;
        return 1;
    }
   // string url = "https://raw.githubusercontent.com/akroutiali/TemperatureController/master/large_trajectory.dat";
    string url = argv[1];
    ClosestTrajectory ct(url);
    vector<Trajectory> trajectories = ct.readTrajectories();
    if(trajectories.empty()) {
        cout<< "trajectories is empty " <<endl;
        return 1;
    }
    int num_point ;
    cout<< "Insert the number of point of the reference trajectory"<<endl;
    cin>>num_point;

    cout<< "Insert the point of the reference trajectory "<<endl;

    vector<Point> points;

    for (int i = 0; i < num_point; i++) {
        Point p;
        std::cin >> p.x >> p.y >> p.t;
        points.push_back(p);
    }
    Trajectory reference(points);
    string metric_type;
    cout<< "Enter the metric type (length or speed )";
    cin >> metric_type;
    if((metric_type != "speed") && (metric_type != "length")) {
        cout<<"the metric_type must be speed or length"<<endl;
        return 1;
    }

    cout<<"Choose the algorithme to use vector, multiset, priority_queue, kdtree" <<endl;
    string algo;
    vector<Trajectory> closest_trajectories;
    cin>> algo;
    if(algo == "vector") {
        closest_trajectories = ct.FindClosestTrajectories(reference, trajectories, metric_type);
    } else if(algo == "multiset") {
        closest_trajectories = ct.FindClosestTrajectories_Ms(reference, trajectories, metric_type);
    } else if(algo == "priority_queue") {
        closest_trajectories = ct.FindClosestTrajectories_Pq(reference, trajectories, metric_type);
    }else if(algo == "kdtree") {
        closest_trajectories = ct.FindClosestTrajectories_FlannTree(reference, trajectories, metric_type);
    } else {
        cout<< "you have to choose an algorithme from the list "<<endl;
        return 1;
    }

    std::cout << "The three closest trajectories to the refernce trajectory " << "(" << reference.length() << ", " << reference.speed()  << ") ";
    std::cout << " according to the " << metric_type << " metric are:" << std::endl;
    for (int i = 0; i < closest_trajectories.size(); i++) {
     Trajectory& t = closest_trajectories[i];
        std::cout << "Trajectory " << i << " (length=" << t.length() << ", speed=" << t.speed() << "): ";
        for (const auto& p : t.get_data()) {
            std::cout << "(" << p.t << ", " << p.x << ", " << p.y << ") ";
        }
        std::cout << std::endl;
    }
    return 0;
}