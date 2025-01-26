const express = require('express');
const { exec } = require('child_process');

const app = express();
const port = 3000;

// Endpoint to list VMs
app.get('/list-vms', (req, res) => {
    exec('./vm_manager', (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            res.status(500).send(`Error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.error(`stderr: ${stderr}`);
            res.status(500).send(`stderr: ${stderr}`);
            return;
        }
        res.send(`<pre>${stdout}</pre>`);  // Send the output from the C program
    });
});

// Endpoint to start a VM
app.get('/start-vm/:name', (req, res) => {
    const vmName = req.params.name;
    exec(`./vm_manager start ${vmName}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            res.status(500).send(`Error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.error(`stderr: ${stderr}`);
            res.status(500).send(`stderr: ${stderr}`);
            return;
        }
        res.send(`<pre>${stdout}</pre>`);
    });
});

// Endpoint to stop a VM
app.get('/stop-vm/:name', (req, res) => {
    const vmName = req.params.name;
    exec(`./vm_manager stop ${vmName}`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error: ${error.message}`);
            res.status(500).send(`Error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.error(`stderr: ${stderr}`);
            res.status(500).send(`stderr: ${stderr}`);
            return;
        }
        res.send(`<pre>${stdout}</pre>`);
    });
});

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
