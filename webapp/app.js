const MAX_DATA_POINTS = 50;
const chart1 = createChart('voltageChart-1');
const chart2 = createChart('voltageChart-2');

function createChart(canvasId) {
    const ctx = document.getElementById(canvasId).getContext('2d');
    return new Chart(ctx, {
        type: 'line',
        data: { labels: [], datasets: [{ label: 'Voltage', data: [], borderColor: '#00e5ff', tension: 0.4 }] },
        options: { responsive: true, maintainAspectRatio: false, scales: { y: { min: 110, max: 130 } } }
    });
}

const mqttClient = new Paho.MQTT.Client("172.20.10.3", 9001, "WebApp_" + Math.random().toString(16).substr(2, 5));

mqttClient.onMessageArrived = function(message) {
    const payload = JSON.parse(message.payloadString);
    const now = new Date().toLocaleTimeString([], { hour12: false });
    if (message.destinationName === "fwncvsd/telemetry/live") {
        const id = payload.node === "TX_01" ? "1" : "2";
        const chart = payload.node === "TX_01" ? chart1 : chart2;
        document.getElementById(`live-voltage-${id}`).innerText = parseFloat(payload.voltage).toFixed(1);
        updateChart(chart, now, payload.voltage);
    }
};

function updateChart(chart, label, data) {
    chart.data.labels.push(label);
    chart.data.datasets[0].data.push(data);
    if (chart.data.labels.length > MAX_DATA_POINTS) { chart.data.labels.shift(); chart.data.datasets[0].data.shift(); }
    chart.update();
}

async function refreshTable() {
    const node = document.getElementById('nodeFilter').value;
    const response = await fetch(`/api/data?node=${node}`);
    const data = await response.json();
    document.getElementById('tableBody').innerHTML = data.map(row => `<tr><td>${row.node}</td><td>${row.voltage}V</td><td>${row.timestamp}</td></tr>`).join('');
}

mqttClient.connect({ onSuccess: () => { mqttClient.subscribe("fwncvsd/#"); } });
setInterval(refreshTable, 2000);
