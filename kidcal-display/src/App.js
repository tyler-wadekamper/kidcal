import React, { useState, useEffect } from "react";

function App() {
  const [weight, setWeight] = useState(null);
  const [ws, setWs] = useState(null);

  useEffect(() => {
    const websocket = new WebSocket("ws://192.168.1.21:8080");

    websocket.onopen = () => {
      console.log("WebSocket Client Connected");
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

  return (
    <div>
      <h1>Weight Data</h1>
      {weight ? <p>Current weight: {weight}</p> : <p>No data yet...</p>}
    </div>
  );
}

export default App;
