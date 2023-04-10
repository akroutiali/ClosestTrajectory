*Install the following Library :

sudo apt-get install -y libcpprest-dev

sudo apt-get install -y libflann-dev

*Compilation using cmake* 
  > execute the command :  cmake .

  aliak@aliak-Vostro-3500:~/MYWORKSPACE/ClosestTrajectory$ cmake .

    -- Configuring done

    -- Generating done

    -- Build files have been written to: /home/aliak/MYWORKSPACE/ClosestTrajectory

  > execute the command : make :

    aliak@aliak-Vostro-3500:~/MYWORKSPACE/ClosestTrajectory$ make

    [ 50%] Built target ClosestTrajectory

    Scanning dependencies of target CTraj

    [ 75%] Building CXX object CMakeFiles/CTraj.dir/main.cpp.o

    [100%] Linking CXX executable CTraj

    [100%] Built target CTraj

    aliak@aliak-Vostro-3500:

*run the binary *

  -After compilation phase a new binary "CTraj" will be generated .
  -you should pass the url of the record file as an argument 

  -URL of large trajectory : https://raw.githubusercontent.com/akroutiali/ClosestTrajectory/master/large_trajectory.dat

  -URL of small trajectory : https://raw.githubusercontent.com/akroutiali/ClosestTrajectory/master/small_trajectory.dat

  -Url of simple trajectory : https://raw.githubusercontent.com/akroutiali/ClosestTrajectory/master/test_trajectory.dat


  -After executing the binary :./CTraj : 
     -you have to enter the number of the point of the traject reference 
     -enter the point of trajectory reference 
     -enter the metric type (speed/length)
     -choose the algorithme "vector, multiset ,priority_queue , kdtree)


aliak@aliak-Vostro-3500:~/MYWORKSPACE/ClosestTrajectory$  ./CTraj https://raw.githubusercontent.com/akroutiali/ClosestTrajectory/master/large_trajectory.dat


Insert the number of point of the reference trajectory

3

Insert the point of the reference trajectory

90 80 70 60 50 40 90 80 70

Enter the metric type (length or speed )

speed

Choose the algorithme to use vector, multiset, priority_queue, kdtree

multiset

The three closest trajectories to the refernce trajectory (42.4264, 1.41421)  according to the speed metric are:

Trajectory 0 (length=16.7703, speed=8.38516): (37, 14, 79) (38, 4, 75) (39, 10, 75)

Trajectory 1 (length=26.5668, speed=8.85559): (31, 56, 84) (32, 59, 94) (33, 60, 85) (34, 59, 78) 

Trajectory 2 (length=20.68, speed=10.34): (84, 83, 68) (85, 93, 69) (86, 100, 77) 

aliak@aliak-Vostro-3500:~/MYWORKSPACE/ClosestTrajectory$

