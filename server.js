
const express = require("express");
const path = require("path");
const app = express();
const PORT = 3000;


app.use(express.static(path.join(__dirname, "public")));

// API route to fetch data from the C++ backend
app.get("/get-songs", async (req, res) => {
    try {
        const response = await fetch("http://localhost:8080/api/songs");
        const data = await response.json();
        res.json(data);
    } catch (error) {
        res.status(500).json({ error: "Failed to connect to C++ backend" });
    }
});


app.listen(PORT, () => {
    console.log(`Frontend running at http://localhost:${PORT}`);
});
