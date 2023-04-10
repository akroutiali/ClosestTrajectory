#include<iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <queue>
#include <set>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <flann/flann.hpp>
//#include <flann/io/hdf5.h>
#include <nanoflann.hpp>

using namespace nanoflann;
using namespace std::chrono;
using namespace std;
using namespace web::http;
using namespace web::http::client;

using namespace Concurrency::streams;

struct Point {
    int x;
    int y;
    int t;
};

/**
 * @brief A class that represents a trajectory of points.
 */

class Trajectory {
public:
    /**
     * @brief Constructs a Trajectory object with the given points.
     * 
     * @param points A vector of Point objects that represents the trajectory.
     */
    Trajectory(const vector<Point>& points);

    /**
     * @brief Destroys the Trajectory object.
     */
    ~Trajectory();

   /**
     * @brief Computes the length of the trajectory.
     * 
     * @return The length of the trajectory as a double.
     */
    double length() const;

    /**
     * @brief Computes the speed of the trajectory.
     * 
     * @return The speed of the trajectory as a double.
     */
    double speed() const;

    /**
     * @brief Computes the distance between two points.
     * 
     * @param a The first point.
     * @param b The second point.
     * 
     * @return The distance between the two points as a double.
     */
    double distance(Point a, Point b) const;

    /**
     * @brief Gets the data points of the trajectory.
     * 
     * @return A reference to the vector of Point objects that represents the trajectory.
     */
    vector<Point>& get_data();

private:
     mutable vector<Point> points_ ;
};

/**
 * @brief A class that reads trajectories from a URL and finds the closest trajectories to a given reference trajectory.
 */
class ClosestTrajectory {
    public:
        /**
        * @brief Constructs a ClosestTrajectory object with the given URL.
        * 
        * @param url_name The URL from which to read the trajectories.
        */
        ClosestTrajectory(const string& url_name);

        /**
        * @brief Destroys the ClosestTrajectory object.
        */
        ~ClosestTrajectory();

        /**
        * @brief Computes the difference in length between two trajectories.
        * 
        * @param t1 The first trajectory.
        * @param t2 The second trajectory.
        * 
        * @return The difference in length between the two trajectories as a double.
        */
        double lengthDifference(const Trajectory& t1,const Trajectory& t2);

        /**
        * @brief Computes the difference in speed between two trajectories.
        * 
        * @param t1 The first trajectory.
        * @param t2 The second trajectory.
        * 
        * @return The difference in speed between the two trajectories as a double.
        */

        double speedDifference(const Trajectory& t1, const Trajectory& t2);
        /**
        * @brief Reads trajectories from the URL.
        * 
        * @return A vector of Trajectory objects that represents the trajectories.
        */
        vector<Trajectory> readTrajectories();

        /**
        * @brief Finds the closest trajectories to a given reference trajectory.
        * 
        * @param reference The reference trajectory.
        * @param trajectories The vector of Trajectory objects to search.
        * @param metric_type The metric type to use for finding the closest trajectories.
        * 
        * @return A vector of Trajectory objects that represents the closest trajectories to the reference trajectory.
        */
        /*I implemented the algorithme findClosestTrajectories using different appproach and data structure
        the taille of the Frame is responsable to choose the efficient algorithme since the time complexity of those algorithme are different
        */
        vector<Trajectory> FindClosestTrajectories( Trajectory& reference,  vector<Trajectory>& trajectories,  string& metric_type);
        vector<Trajectory> FindClosestTrajectories_Pq( Trajectory& reference,  vector<Trajectory>& trajectories,  string& metric_type);
        vector<Trajectory> FindClosestTrajectories_Ms(Trajectory& reference, vector<Trajectory>& trajectories, string& metric_type);
        vector<Trajectory> FindClosestTrajectories_FlannTree(Trajectory& reference, vector<Trajectory>& trajectories, string& metric_type);
    private:
        string url_; 
};

//sudo apt-get install libflann-dev