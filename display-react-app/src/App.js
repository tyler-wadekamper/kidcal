import React, { useState, useEffect } from "react";

const CONVERSION_FACTOR = 49400;

function App() {
  const [weight, setWeight] = useState(null);
  const [tareOffset, setTareOffset] = useState(992000);
  const [convertedWeight, setConvertedWeight] = useState({
    pounds: 0,
    ounces: 0,
  });
  const [ws, setWs] = useState(null);

  useEffect(() => {
    const weightInPounds = (weight - tareOffset) / CONVERSION_FACTOR;
    const wholePounds = Math.floor(weightInPounds);
    const ounces = Math.round((weightInPounds - wholePounds) * 16);

    setConvertedWeight({
      pounds: wholePounds,
      ounces: ounces,
    });
  }, [weight, tareOffset]);

  useEffect(() => {
    const websocket = new WebSocket("wss://67.185.149.38:80");

    websocket.onopen = () => {
      console.log("Secure WebSocket Client Connected");
    };

    websocket.onmessage = (message) => {
      console.log("Received: " + message.data);
      setWeight(message.data);
    };

    setWs(websocket);

    return () => {
      websocket.close();
    };
  }, []);

  function handleTareScale(e) {
    setTareOffset(weight);
  }

  return (
    <div class="container">
      <h1>Kidcal</h1>
      {weight ? (
        <p>
          {convertedWeight.pounds} lbs {convertedWeight.ounces} oz
        </p>
      ) : (
        <p>No data yet...</p>
      )}
      <button onClick={handleTareScale}>Tare</button>
      <button>Capture</button>
    </div>
  );
}

export default App;
