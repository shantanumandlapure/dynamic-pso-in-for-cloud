🧠 Cloud Resource Allocation using Particle Swarm Optimization (PSO)

🚀 Overview
This project applies swarm intelligence techniques to optimize resource allocation in cloud computing environments. Using the Particle Swarm Optimization (PSO) algorithm implemented in C, the system dynamically assigns computational tasks to servers.

The goals are to:

⚖️ Balance server loads

♻️ Minimize resource wastage

⚡ Improve overall system performance and energy efficiency

✨ Key Features
🧬 AI-Powered Optimization: Implements the PSO algorithm for intelligent task allocation.

🔄 Dynamic Resource Allocation: Adjusts task assignments in real-time based on server load.

📊 Comprehensive Metrics: Evaluates system performance with respect to:

Resource utilization

Response time

Energy consumption and efficiency

Throughput

🎯 Objectives
Achieve load balancing across distributed servers

Enhance energy efficiency during task processing

Minimize response time for computational tasks

🧩 PSO Particle Structure
Each particle in the swarm represents a potential allocation solution:

🧾 Assignment Array: Task-to-server mapping

🧠 Fitness Value: Efficiency score based on system objectives

🏅 Personal Best: Best-known assignment by the particle

🔧 Initialization
Particles are initialized with random task-server assignments, ensuring a diverse search space for effective optimization.

📈 Velocity & Position Updates
Particles update their velocity and position using standard PSO equations, considering:

Their personal best solution

The global best found by the swarm

Dynamic server load and capacity constraints

📐 Fitness Evaluation
Each particle's fitness is calculated based on:

🔌 Resource Wastage: Unused server capacity

🖥️ CPU & Memory Utilization

📦 Task Requirements vs. Server Capabilities

📊 Performance Metrics
Metric	Description
Resource Utilization	% of server capacity used
Total Response Time	Time to process all tasks
Energy Consumed	Energy used for task execution
Energy Efficiency	Tasks processed per energy unit
Throughput	Tasks processed per second

📈 Results & Analysis
The system's performance is analyzed based on the above metrics. Results demonstrate how PSO enhances:

Load balancing

Resource efficiency

System throughput and energy-aware task allocation

🛠 Tech Stack
Language: C

Algorithm: Particle Swarm Optimization (PSO)

Domain: Cloud Computing / Resource Management / Optimization

🧪 Future Improvements
Support for heterogeneous servers

Integration with real-time cloud infrastructure (e.g., OpenStack API)

Multi-objective optimization (Pareto front)

