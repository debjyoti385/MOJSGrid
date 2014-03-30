MOJSGrid (Multi-objective Evolution based Dynamic Job Scheduler in Grid)
========================================================================
## Compile: ##
### Extract “modjs.zip” ###
```
	$ tar -xvzf modjs.zip
```
### Compile the source code ###
```
$ make
```
### Go to folder “resource” ###
```
$ cd resource
$ gcc -c resource manager.c -o resource
```
Enter available resource information in “initial resource.in” in specified format.
### Copy any job file from ”data” folder and name it ”job.in” for e.g. ###
```
$ cp data/DAS-2/job no pred.in job.in
```
Prepare hostfile.txt with ip address or domain name of all resources.


### Command to run the job scheduler follows : ###
```
mpiexec -disable-hostname-propagation -hostfile hostfile.txt ./modjsg <plot>
<NUM JOBS> <POPULATION> <GENERATION> <RANDOM INTEGER>
```
• <plot> - 0 (run without GNUPLOT), 1 (without GNUPLOT)
• <NUM JOBS> - Number of jobs to be scheduled on each run of the job
scheduler module. Range [100,1000] multiplier of 4.
• <POPULATION> - Chromosome population for genetic algorithm range,
Range [200,1000]
• <GENERATION> - Iteration in genetic algorithm, Range [50,300]
• <RANDOM INTEGER> - Any integer for randomize function seed.
e.g. 
```
$ mpiexec -disable-hostname-propagation -hostfile hostfile.txt ./modjsg 0 100 400 300 432421
```

### Command to run resource manager : ###
```
$ cd resource
$ ./resource
```

To configure the selection criteria of schedule after final population of efficient schedule has been generated modify weight fun[] in report best() function in report.c file.

##### Note: #####
Data sets are in other location 
