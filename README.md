**## Description**
This project aims to optimize resource allocation in cloud computing environments using swarm intelligence techniques. By implementing the Particle Swarm Optimization (PSO) algorithm in C, the system efficiently allocates computational tasks to servers. The primary goals are to minimize resource wastage, balance server loads, and improve overall system performance.

**## Features**
- **AI Algorithms**: Implements PSO for dynamic resource allocation.
- **Dynamic Resource Allocation**: Adjusts server capacities based on real-time load monitoring.
- **Performance Metrics**: Evaluates resource utilization, response time, energy consumption, energy efficiency, and throughput.

**## Objectives**
The project focuses on:
1. Load balancing across servers.
2. Enhancing energy efficiency.
3. Minimizing response time for task processing.

**## Particle Representation**
Each particle represents a potential solution to the task allocation problem:
- **Assignment Array**: Maps tasks to servers.
- **Fitness Value**: Measures the efficiency of the allocation.
- **Personal Best Assignment**: Best assignment for each particle during optimization.

**## Initialization**
Particles are initialized with random task assignments to servers, ensuring diverse starting points for the optimization process.

**## Velocity and Position Updates**
Particles' velocities and positions are updated using PSO equations, considering multiple objectives and dynamic server capacities.

**## Fitness Evaluation**
The fitness of each particle is evaluated based on:
- Resource wastage across servers.
- CPU and memory utilization.
- Task requirements and server capacities.

**## Metrics Collected**
- **Resource Utilization**: Percentage of server capacity used.
- **Total Response Time**: Time taken to process all tasks.
- **Total Energy Consumed**: Energy required to process all tasks.
- **Energy Efficiency**: Number of tasks processed per unit of energy.
- **Throughput**: Number of tasks processed per second.

**## Results Analysis**
The collected metrics are analyzed to evaluate the performance of the PSO algorithm with respect to the optimization objectives.
