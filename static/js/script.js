// Fetch sensor data every 2 seconds
async function updateSensorData() {
    try {
        const response = await fetch('/get_sensor_data');
        const data = await response.json();

        document.getElementById('temp').innerText = data.temperature.toFixed(1);
        document.getElementById('hum').innerText = data.humidity.toFixed(1);
        document.getElementById('fire').innerText = data.fire ? "YES 🔥" : "NO";
        document.getElementById('door').innerText = data.door.toUpperCase();

    } catch (error) {
        console.error("Error fetching sensor data:", error);
    }
}

setInterval(updateSensorData, 500);
updateSensorData(); // initial call

// Open door button
document.getElementById('openDoorBtn').addEventListener('click', async () => {
    try {
        const response = await fetch('/open_door', { method: 'POST' });
        const result = await response.json();
        console.log(result.status);
    } catch (error) {
        console.error("Error sending door command:", error);
    }
});