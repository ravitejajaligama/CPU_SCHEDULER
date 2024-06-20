# CPU_SCHEDULER
Project description: This project is a CPU SCHEDULER. It allows
the users to stimulate different scheduling algorithms and
analyze the performance of the particular selected algorithm.
Also, it suggests the optimal algorithm for a particular set of
processes.


How to run the project:

To clone my repository and work on it locally, you can use the following steps:

1) Open your terminal or command prompt.

2) Navigate to the directory where you want to clone the repository.

3) Run the following command to clone the repository: 
git clone https://github.com/ravitejajaligama/CPU_SCHEDULER.git
	
4) Navigate into the cloned repository:  cd CPU_SCHEDULER
	
5) Open the project in your preferred code editor:
     For example, if you're using Visual Studio Code, you can run:  code .

Now once all the files are displayed in the IDE follow the below steps:

Step 1: Install Node.js
If you haven't already, download and install Node.js for your
operating system.

Step 2: Install project dependencies
Navigate to your project directory in the terminal and run the
following command to install the required dependencies:
• npm install express

Step 3: Compile the C++ code
Compile scheduler.cpp into an executable file. Use a C++
compiler like g++:
• g++ -o scheduler.exe scheduler.cpp

Step 4: Run the Express server
Start the Express server by running the following command in
your project directory:
node server.js

Step 5: Access the simulator
Open a web browser and go to http://localhost:3000 to access
the CPU scheduler simulator.

Step 6: Simulate scheduling
Fill in the details of the processes, select a scheduling algorithm,
and click "Simulate" to run the simulation. The results will be
displayed on the webpage.


Internal working of the project:
1. Client-Side (Frontend):
o The user interacts with the frontend to input the
number of processes, arrival time, burst time, priority,
and scheduling algorithm details.
o This information is sent to the server for simulation.

2. Server-Side (Backend):
o The server receives the simulation request from the
client and prepares an input file (input.txt) containing
the process details.
o It then compiles the scheduler.cpp file into an
executable (scheduler.exe) using the g++ compiler.
o The compiled executable is executed with input
redirection from input.txt.
o The output of the simulation is parsed and sent back to
the client for display.

Learning Takeaways from the Project:

1. Integration of Frontend and Backend: I learned how to
integrate C++ code with a web server using Node.js,
allowing a web interface to interact with backend processes
and execute C++ programs.

2.Understanding Scheduling Algorithms: Working on the
scheduler.cpp file deepened my understanding of various
CPU scheduling algorithms like FCFS, SJF, priority
scheduling, and round-robin, and how to implement and
test them.

3.Web Development: I enhanced my web development skills
by creating a user-friendly frontend interface using HTML,
CSS, and JavaScript, which communicates effectively with
the backend server.

➢In some situations I used Chatgpt:
▪ To make my cpp code look readable.
▪ I was having some confusions in integrating the front end
and backend using node.js
▪ I encountered many errors in between such as
path/directory related errors. For that error handling I
used chatgpt to encounter it.


Resources and References used:
• I learned about CPU SCHEDULING and the scheduling
algorithms from youtube. The playlist which was sent to us in
the discord.
• I was aware of html and some basics of css earlier. But I have
learnt javascript and node.js basics again from youtube. I
enhanced the codes with the help of chatgpt.
• ChatGPT for assistance with error handling, integration of
frontend and backend, and overall project guidance.
