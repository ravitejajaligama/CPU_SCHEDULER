const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(express.json());
app.use(express.static(path.join(__dirname, '../client'))); 


app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../client/index.html'));
});


app.post('/simulate', (req, res) => {
    const { processes, algorithm, preemptive, quantum } = req.body;

    console.log('Received simulation request:', req.body);

    if (!req.body) {
        console.error('Request body is undefined');
        res.status(400).send('Request body should be defined');
        return;
    }

    if (!processes || !Array.isArray(processes)) {
        console.error('Processes is not an array or undefined:', processes);
        res.status(400).send('Processes should be an array');
        return;
    }

    console.log('Number of processes:', processes.length);

    const inputFile = path.join(__dirname, 'input.txt');
    let inputData = `${processes.length}\n`;
    processes.forEach(p => {
        inputData += `${p.pid} ${p.arrivaltime} ${p.bursttime} ${p.priority}\n`;
    });
    inputData += `${algorithm}\n${preemptive ? 1 : 0}\n${quantum}\n`;

    fs.writeFileSync(inputFile, inputData);

    const cppFilePath = path.join(__dirname, 'scheduler.cpp');
    const exeFilePath = path.join(__dirname, 'scheduler.exe');

    exec(`g++ -o "${exeFilePath}" "${cppFilePath}"`, (compileError, compileStdout, compileStderr) => {
        if (compileError || compileStderr) {
            console.error(`Error compiling C++ program: ${compileError || compileStderr}`);
            res.status(500).send('Internal server error');
            return;
        }

        exec(`"${exeFilePath}" < "${inputFile}"`, (runError, stdout, stderr) => {
            if (runError || stderr) {
                console.error(`Error executing C++ program: ${runError || stderr}`);
                res.status(500).send('Internal server error');
                return;
            }

            console.log(`C++ program output: ${stdout}`);

            
            const lines = stdout.split('\n');
            const results = [];
            let i = 5; 
            while (i < lines.length && lines[i].startsWith('|')) {
                const columns = lines[i].split('|').map(col => col.trim());
                if (columns.length >= 6) {
                    results.push({
                        pid: parseInt(columns[1]),
                        arrivaltime: parseInt(columns[2]),
                        bursttime: parseInt(columns[3]),
                        waiting_time: parseInt(columns[4]),
                        turnaround_time: parseInt(columns[5]),
                        completion_time: parseInt(columns[6])
                    });
                }
                i++;
            }

            const avg_waiting_time = parseFloat(lines[lines.length - 3].split(': ')[1]);
            const avg_turnaround_time = parseFloat(lines[lines.length - 2].split(': ')[1]);
            const optimal_algorithm = lines[lines.length - 1].split(': ')[1];

           
            fs.unlinkSync(exeFilePath);

            res.json({
                results,
                avg_waiting_time,
                avg_turnaround_time,
                optimal_algorithm
            });
        });
    });
});


const port = 3000;
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
