#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Define constants and data structures
#define NUM_SERVERS 4
#define NUM_TASKS 10
#define NUM_PROGRAMS 10
#define NUM_PARTICLES 50
#define MAX_ITERATIONS 2000
#define INERTIA_WEIGHT 0.7
#define COGNITIVE_WEIGHT 1.5
#define SOCIAL_WEIGHT 1.5
#define MAX_CAPACITY 500
#define MIN_CAPACITY 100
#define MIN_TASK_REQUIREMENT 10
#define MAX_TASK_REQUIREMENT 50
#define MIN_CPU 10
#define MAX_CPU 50
#define MIN_MEMORY 5
#define MAX_MEMORY 30
#define MIN_EXECUTION_TIME 1
#define MAX_EXECUTION_TIME 10

typedef struct {
    int id;
    double capacity;
    double currentLoad;
    double cpuUtilization;
    double memoryUtilization;
} Server;

typedef struct {
    int id;
    double requiredResources;
} Task;

typedef struct {
    int id;
    double cpuRequirements;
    double memoryRequirements;
    double executionTime;
} Program;

typedef struct {
    int assignment[NUM_TASKS];
    double fitness;
    int personalBestAssignment[NUM_TASKS];
} Particle;

Server servers[NUM_SERVERS];
Task tasks[NUM_TASKS];
Particle particles[NUM_PARTICLES];
Particle globalBest;



Program programs[NUM_PROGRAMS];
double evaluateFitness(Particle *particle);

void initialize() {
    srand(time(NULL));

    // Initialize servers with random capacities within constraints
    for (int i = 0; i < NUM_SERVERS; ++i) {
        servers[i].id = i;
        servers[i].capacity = MIN_CAPACITY + (rand() % (MAX_CAPACITY - MIN_CAPACITY + 1));
        servers[i].currentLoad = 0.0;
        servers[i].cpuUtilization = 0.0;
        servers[i].memoryUtilization = 0.0;
    }

    // Initialize tasks with random required resources within constraints
    for (int i = 0; i < NUM_TASKS; ++i) {
        tasks[i].id = i;
        tasks[i].requiredResources = MIN_TASK_REQUIREMENT + (rand() % (MAX_TASK_REQUIREMENT - MIN_TASK_REQUIREMENT + 1));
    }

    // Initialize programs with random resource requirements and execution times
    for (int i = 0; i < NUM_PROGRAMS; ++i) {
        programs[i].id = i;
        programs[i].cpuRequirements = MIN_CPU + (rand() % (MAX_CPU - MIN_CPU + 1));
        programs[i].memoryRequirements = MIN_MEMORY + (rand() % (MAX_MEMORY - MIN_MEMORY + 1));
        programs[i].executionTime = MIN_EXECUTION_TIME + (rand() % (MAX_EXECUTION_TIME - MIN_EXECUTION_TIME + 1));
    }

    // Initialize particles with random assignments
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        for (int j = 0; j < NUM_TASKS; ++j) {
            particles[i].assignment[j] = rand() % NUM_SERVERS;
        }
        particles[i].fitness = evaluateFitness(&particles[i]);
        for (int j = 0; j < NUM_TASKS; ++j) {
            particles[i].personalBestAssignment[j] = particles[i].assignment[j];
        }
    }

    // Initialize global best particle
    globalBest.fitness = -INFINITY;
}


double evaluateFitness(Particle *particle) {
    // Calculate the total resource wastage across all servers
    double totalWastage = 0.0;

    // Calculate the load on each server based on task assignments
    for (int i = 0; i < NUM_SERVERS; ++i) {
        servers[i].currentLoad = 0.0;
        servers[i].cpuUtilization = 0.0;
        servers[i].memoryUtilization = 0.0;
    }

    for (int i = 0; i < NUM_TASKS; ++i) {
        int serverId = particle->assignment[i];
        double taskRequirements = tasks[i].requiredResources;
        double serverCapacity = servers[serverId].capacity;

        // Update server load and calculate wastage
        servers[serverId].currentLoad += taskRequirements;
        double wastage = fmax(0, servers[serverId].currentLoad - serverCapacity);
        totalWastage += wastage;

        // Update server CPU and memory utilization
        servers[serverId].cpuUtilization += programs[i % NUM_PROGRAMS].cpuRequirements;
        servers[serverId].memoryUtilization += programs[i % NUM_PROGRAMS].memoryRequirements;
    }


    // Calculate CPU and Memory Utilization as a percentage
    for (int i = 0; i < NUM_SERVERS; ++i) {
        servers[i].cpuUtilization = (servers[i].cpuUtilization / servers[i].capacity) * 100.0;
        servers[i].memoryUtilization = (servers[i].memoryUtilization / servers[i].capacity) * 100.0;
    }


    // Fitness is the negative of total wastage (minimize wastage)
    double fitness = -totalWastage;

    return fitness;
}

void dynamicResourceAllocation() {
    // Monitor server loads and adjust capacities dynamically
    for (int i = 0; i < NUM_SERVERS; ++i) {
        servers[i].cpuUtilization = servers[i].cpuUtilization / servers[i].capacity;
        servers[i].memoryUtilization = servers[i].memoryUtilization / servers[i].capacity;

        if (servers[i].cpuUtilization > 0.70) {
            servers[i].capacity += 10; // Increase capacity
        } else if (servers[i].cpuUtilization < 0.2) {
            servers[i].capacity -= 10; // Decrease capacity
        }

        // Ensure capacity remains within constraints
        servers[i].capacity = fmax(MIN_CAPACITY, fmin(MAX_CAPACITY, servers[i].capacity));
    }
}

void updateParticle(Particle *particle) {

    //These random values are used to introduce stochasticity into the PSO algorithm 
    //The values are used to compute the cognitive and social components of the velocity update

    double r1 = (double)rand() / RAND_MAX;
    double r2 = (double)rand() / RAND_MAX;

    for (int i = 0; i < NUM_TASKS; ++i) {
        double inertiaTerm = INERTIA_WEIGHT * particle->personalBestAssignment[i];
        double cognitiveTerm = COGNITIVE_WEIGHT * r1 * (particle->personalBestAssignment[i] - particle->assignment[i]);
        double socialTerm = SOCIAL_WEIGHT * r2 * (globalBest.assignment[i] - particle->assignment[i]);

//three terms together contribute to the calculation of velocityUpdate, which represents how the particle should adjust its task assignments (position) in the solution space 
//The balance between these terms guides the particle's movement as it seeks to explore and exploit the solution space to find better assignments that optimize the given fitness function

        double velocityUpdate = inertiaTerm + cognitiveTerm + socialTerm;
        double newAssignment = particle->assignment[i] + velocityUpdate;

        // Ensure that newAssignment respects server capacities and constraints
        if (newAssignment < 0) {
            newAssignment = 0;
        } else if (newAssignment >= NUM_SERVERS) {
            newAssignment = NUM_SERVERS - 1;
        }

        particle->assignment[i] = (int)newAssignment;
    }

    // Update personal best assignment if needed
    double currentFitness = evaluateFitness(particle);
    if (currentFitness > particle->fitness) {
        particle->fitness = currentFitness;
        for (int i = 0; i < NUM_TASKS; ++i) {
            particle->personalBestAssignment[i] = particle->assignment[i];
        }
    }

    // Update global best if needed
    if (currentFitness > globalBest.fitness) {
        globalBest.fitness = currentFitness;
        for (int i = 0; i < NUM_TASKS; ++i) {
            globalBest.assignment[i] = particle->assignment[i];
        }
    }
}



int main() {
    initialize();
    clock_t start, end;
    double totalEnergyConsumed = 0.0;
    
    // Start measuring time
    start = clock();

    for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            updateParticle(&particles[i]);
           
        }

        // Implement dynamic resource allocation at appropriate intervals
        if (iteration % 100 == 0) {
            dynamicResourceAllocation();
        }
    }

    // Stop measuring time
    end = clock();
    double totalResponseTime = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Calculate throughput
    int tasksProcessed = 0;
    for (int i = 0; i < NUM_TASKS; ++i) {
        int serverId = globalBest.assignment[i];
        double taskRequirements = tasks[i].requiredResources;
        double serverCapacity = servers[serverId].capacity;

        if (serverCapacity - servers[serverId].currentLoad >= taskRequirements) {
            tasksProcessed++;
        }

        // Calculate energy consumed for this task (you can adjust this value)
        double taskEnergy = 0.1 * taskRequirements; // Change 0.1 to an appropriate constant
        totalEnergyConsumed += taskEnergy;
    }

    double throughput = tasksProcessed / totalResponseTime;

    // Calculate energy efficiency
    double energyEfficiency = throughput / totalEnergyConsumed;

  // Output the best assignment found
printf("Best Assignment:\n");
for (int i = 0; i < NUM_TASKS; ++i) {
    int taskServerId = globalBest.assignment[i];
    printf("Task %d -> Server %d\n", i, taskServerId);

    // Update server resource usage based on program allocation
    servers[taskServerId].currentLoad += programs[i].cpuRequirements; // Change programId to i

    // Output resource utilization
    printf("CPU Utilization on Server %d: %.2f%%\n", taskServerId,
           (servers[taskServerId].cpuUtilization));
    printf("Memory Utilization on Server %d: %.2f%%\n", taskServerId,
           (servers[taskServerId].memoryUtilization));
    printf("Execution Time on Server %d: %.2f seconds\n\n", taskServerId, programs[i].executionTime); // Change programId to i
}
    printf("Fitness: %f\n", globalBest.fitness);

    // Calculate resource utilization
    double totalResourcesUsed = 0.0;
    double totalServerCapacity = 0.0;

    for (int i = 0; i < NUM_TASKS; ++i) {
        int serverId = globalBest.assignment[i];
        double taskRequirements = tasks[i].requiredResources;
        totalResourcesUsed += taskRequirements;
    }

    for (int i = 0; i < NUM_SERVERS; ++i) {
        totalServerCapacity += servers[i].capacity;
    }

    double resourceUtilizationPercentage = (totalResourcesUsed / totalServerCapacity) * 100.0;

    // Output additional metrics
    printf("Resource Utilization: %.2f%%\n", resourceUtilizationPercentage);
    printf("Total Response Time: %.2f seconds\n", totalResponseTime);
    printf("Total Energy Consumed: %.2f units\n", totalEnergyConsumed);
    printf("Energy Efficiency: %.2f tasks/Joule\n", energyEfficiency);
    printf("Throughput: %.2f tasks/second\n\n", throughput);

    return 0;
}
