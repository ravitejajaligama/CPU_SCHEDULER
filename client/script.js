document.addEventListener('DOMContentLoaded', function() {
    const processCountInput = document.getElementById('processCount');
    const generateProcessesBtn = document.getElementById('generateProcessesBtn');
    const processDetails = document.getElementById('processDetails');
    const algorithmSelect = document.getElementById('algorithm');
    const preemptiveInput = document.getElementById('preemptiveInput');
    const preemptiveCheckbox = document.getElementById('preemptive');
    const quantumInput = document.getElementById('quantumInput');
    const quantum = document.getElementById('quantum');
    const simulateBtn = document.getElementById('simulateBtn');
    const resultsDiv = document.getElementById('results');
    
    generateProcessesBtn.addEventListener('click', function() {
        const processCount = parseInt(processCountInput.value);
        if (isNaN(processCount) || processCount <= 0) {
            alert('Please enter a valid number of processes.');
            return;
        }
    
        processDetails.innerHTML = '';
        for (let i = 0; i < processCount; i++) {
            const processDiv = document.createElement('div');
            processDiv.className = 'input-container';
            processDiv.innerHTML = `
                <h3>Process ${i + 1}</h3>
                <label for="arrivalTime${i}">Arrival Time:</label>
                <input type="number" id="arrivalTime${i}" name="arrivalTime${i}" required>
                <label for="burstTime${i}">Burst Time:</label>
                <input type="number" id="burstTime${i}" name="burstTime${i}" required>
                <label for="priority${i}">Priority:</label>
                <input type="number" id="priority${i}" name="priority${i}" required>
            `;
            processDetails.appendChild(processDiv);
        }
        processDetails.classList.remove('hidden');
    });
    
    algorithmSelect.addEventListener('change', function() {
        const selectedAlgorithm = parseInt(algorithmSelect.value);
        if (selectedAlgorithm === 2 || selectedAlgorithm === 3) {
            preemptiveInput.classList.remove('hidden');
        } else {
            preemptiveInput.classList.add('hidden');
            preemptiveCheckbox.checked = false;
        }
        if (selectedAlgorithm === 4) {
            quantumInput.classList.remove('hidden');
        } else {
            quantumInput.classList.add('hidden');
        }
    });
    
    simulateBtn.addEventListener('click', function() {
        const processCount = parseInt(processCountInput.value);
        const selectedAlgorithm = parseInt(algorithmSelect.value);
        const preemptive = preemptiveCheckbox.checked;
        const quantumValue = parseInt(quantum.value);
    
        if (isNaN(processCount) || processCount <= 0) {
            alert('Please enter a valid number of processes.');
            return;
        }
    
        if (selectedAlgorithm === 4 && (isNaN(quantumValue) || quantumValue <= 0)) {
            alert('Please enter a valid time quantum.');
            return;
        }
    
        let processes = [];
        for (let i = 0; i < processCount; i++) {
            const arrivalTimeElement = document.getElementById(`arrivalTime${i}`);
            const burstTimeElement = document.getElementById(`burstTime${i}`);
            const priorityElement = document.getElementById(`priority${i}`);

            if (!arrivalTimeElement.value || !burstTimeElement.value || !priorityElement.value) {
                alert('Please make sure all process fields are filled.');
                return;
            }

            const arrivalTime = parseInt(arrivalTimeElement.value);
            const burstTime = parseInt(burstTimeElement.value);
            const priority = parseInt(priorityElement.value);

            processes.push({
                pid: i + 1,
                arrivaltime: arrivalTime,
                bursttime: burstTime,
                priority: priority
            });
        }

    
        fetch('/simulate', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                processes: processes,
                algorithm: selectedAlgorithm,
                preemptive: preemptive,
                quantum: selectedAlgorithm === 4 ? quantumValue : undefined
            })
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            displayResults(data);
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Failed to retrieve simulation results.');
        });
    });
    
    
    function displayResults(data) {
        resultsDiv.innerHTML = `
            <h2>Simulation Results</h2>
            <table>
                <thead>
                    <tr>
                        <th>PID</th>
                        <th>Arrival Time</th>
                        <th>Burst Time</th>
                        <th>Priority</th>
                        <th>Waiting Time</th>
                        <th>Turnaround Time</th>
                    </tr>
                </thead>
                <tbody>
                    ${data.results.map(process => `
                        <tr>
                            <td>${process.pid}</td>
                            <td>${process.arrivaltime}</td>
                            <td>${process.bursttime}</td>
                            <td>${process.priority}</td>
                            <td>${process.waiting_time}</td>
                            <td>${process.turnaround_time}</td>
                        </tr>
                    `).join('')}
                </tbody>
            </table>
            <p>Average Waiting Time: ${data.avg_waiting_time.toFixed(2)}</p>
            <p>Average Turnaround Time: ${data.avg_turnaround_time.toFixed(2)}</p>
            <p>Optimal Algorithm: ${data.optimal_algorithm}</p>
        `;
        resultsDiv.classList.remove('hidden');
    }
});
