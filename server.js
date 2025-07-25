const WebSocket = require('ws');
const express = require('express');
const app = express();
const wss = new WebSocket.Server({ port: 8080 });

app.use(express.static('public'));

const clients = new Map();

wss.on('connection', (ws, req) => {
    const clientId = req.url.split('=')[1] || 'unknown';
    clients.set(clientId, ws);
    
    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            // Broadcast status to all clients
            wss.clients.forEach(client => {
                if (client.readyState === WebSocket.OPEN) {
                    client.send(JSON.stringify(data));
                }
            });
        } catch (error) {
            console.error('Error parsing message:', error);
        }
    });
    
    ws.on('close', () => {
        clients.delete(clientId);
    });
});

app.listen(3000, () => {
    console.log('Server running on port 3000');
});
