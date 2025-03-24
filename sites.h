const char* wifiCredentialsPage = R"rawliteral(
<html>
  <head>
    <style>
      body {
        background-color: #333; /* Dark grey background */
        color: #fff; /* White text for good contrast */
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
        font-family: Arial, sans-serif; /* Ensure consistent font */
      }
      .container {
        background-color: #d3d3d3;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);
      }
      .content {
        border: 2px solid #ccc; /* Light grey border */
        padding: 20px;
        border-radius: 10px; /* Optional: rounded corners for aesthetic touch */
        background-color: #444; /* Slightly lighter grey for the content background */
        text-align: center; /* Center-align the text inside */
      }
      input[type="password"] {
        -webkit-text-security: disc;
        text-security: disc;
      }
    </style>
  </head>
  <body>
    <div class="container content">
      <h1>WLAN Daten eintragen</h1>
      <form action="/save" method="POST">
        <label for="ssid">SSID:</label><br>
        <input type="text" id="ssid" name="ssid"><br><br>
        <label for="password">Passwort:</label><br>
        <input type="password" id="password" name="password"><br><br>
        <input type="submit" value="Speichern">
      </form>
    </div>
  </body>
</html>
)rawliteral";

const char* connectingPage = R"rawliteral(
<html>
  <head>
    <style>
      body {
        background-color: #333; /* Dark grey background */
        color: #fff; /* White text for good contrast */
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
        font-family: Arial, sans-serif; /* Ensure consistent font */
      }
      .content {
        border: 2px solid #ccc; /* Light grey border */
        padding: 20px;
        border-radius: 10px; /* Optional: rounded corners for aesthetic touch */
        background-color: #444; /* Slightly lighter grey for the content background */
        text-align: center; /* Center-align the text inside */
      }
    </style>
  </head>
  <body>
    <div class="content">
      <h1>Daten gespeichert!</h1>
      <p>Versuche, eine Verbindung zum Netzwerk herzustellen...</p>
      <p>%IP%</p>
    </div>
  </body>
</html>
)rawliteral";

const char* mainPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP8266 Uhrzeit</title>
  <style>
    body {
      background-color: #333; /* Dark grey background */
      color: #fff; /* White text for good contrast */
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }
    .content {
      border: 2px solid #ccc; /* Light grey border */
      padding: 20px;
      border-radius: 10px; /* Optional: rounded corners for aesthetic touch */
      background-color: #444; /* Slightly lighter grey for the content background */
      text-align: center; /* Center-align the text inside */
    }
    .confetti {
      position: absolute; /* Position relative to the entire page */
      top: -10px; /* Start slightly above the viewport */
      width: 40px; /* Stretch width */
      height: 20px; /* Stretch height */
      font-size: 14px;
      font-weight: bolder;
      line-height: 40px; /* Center "1337" text vertically */
      text-align: center; /* Center text horizontally */
      color: #000; /* black text for visibility */
      animation: fall 3s linear infinite;
      opacity: 0;
      z-index: 1000;
      border-radius: 5px; /* Optional: Round the corners slightly */
    }
    @keyframes fall {
      0% {
        transform: translateY(-100%);
        opacity: 1;
      }
      100% {
        transform: translateY(120vh) rotate(360deg);
        opacity: 0;
      }
    }
  </style>
</head>
<body>
  <div class="content">
    <h1>Aktuelle Uhrzeit:</h1>
    <p id="time">%TIME%</p>
    <h3>Diagnose:</h3>
    <p id="ram">%RAM%</p>
  </div>
  <script>
    setInterval(function() {
      fetch('/ram')
        .then(response => response.text())
        .then(data => document.getElementById('ram').innerText = data);
      fetch('/time')
        .then(response => response.text())
        .then(data => {
          document.getElementById('time').innerText = data;

          // Trigger confetti if time is "13:37"
          if (data.includes("13:37")) {
            triggerConfetti();
          }
        });
    }, 1000);

    function triggerConfetti() {
     for (let i = 0; i < 20; i++) { // Fewer confetti elements
        let confetti = document.createElement('div');
        confetti.className = 'confetti';
        confetti.innerText = '1337'; // Add "1337" text inside each confetti
        confetti.style.left = Math.random() * 100 + 'vw';
        confetti.style.color = getRandomColor(); // Set text color to a random color
        confetti.style.animationDelay = Math.random() * 2 + 's';

        // Add random rotation
        const rotation = Math.random() * 360; // Random degree of rotation
        confetti.style.transform = `rotate(${rotation}deg)`; // Set initial rotation

        document.body.appendChild(confetti);

        // Remove confetti after animation
        setTimeout(() => confetti.remove(), 3000);
      }
    }

    function getRandomColor() {
      const colors = ['#ff5733', '#33ff57', '#3357ff', '#ff33a8', '#f7e600'];
      return colors[Math.floor(Math.random() * colors.length)];
    }
  </script>
</body>
</html>)rawliteral";
