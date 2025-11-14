"use client";

import { useState } from "react";

declare global {
    interface Window {
        synthAPI: {
            ports: () => Promise<string[]>;
            openPort: (port: number) => Promise<void>;
            setAmplitude: (value: number) => Promise<void>;
            startRecording: () => Promise<void>;
            stopRecording: () => Promise<void>;
        };
    }
}

export default function PlayPage() {
    const [status, setStatus] = useState<string>("idle");
    const [midiPorts, setMidiPorts] = useState<string[]>([]);
    const [selectedPort, setSelectedPort] = useState<number | null>(null);

    const listPorts = async () => {
        setStatus("Fetching MIDI ports...");
        const ports = await window.synthAPI.ports();
        setMidiPorts(ports);
        setStatus(ports.length ? `Found ${ports.length} MIDI port(s)` : "No MIDI ports found");
    };

    const openPort = async (port: number) => {
        setSelectedPort(port);
        setStatus(`Opening port ${port}...`);
        await window.synthAPI.openPort(port);
        setStatus(`Port ${port} opened`);
    };

    const startRecording = async () => {
        setStatus("Recording started...");
        await window.synthAPI.startRecording();
    };

    const stopRecording = async () => {
        setStatus("Recording stopped.");
        await window.synthAPI.stopRecording();
    };

    return (
        <main>
            <h1>🎛️ Simple Synth Controller</h1>
            <p>Status: {status}</p>

            <div>
                <button onClick={listPorts}>List MIDI Ports</button>
                <ul>
                    {midiPorts.map((port, i) => (
                        <li key={i}>
                            {port}{" "}
                            <button onClick={() => openPort(i)}>
                                Open Port {i}
                            </button>
                        </li>
                    ))}
                </ul>
            </div>

            {/* 🎤 Recording Controls */}
            <div style={{ marginTop: "20px" }}>
                <button onClick={startRecording}>Start Recording</button>
                <button onClick={stopRecording} style={{ marginLeft: "10px" }}>
                    Stop Recording
                </button>
            </div>
        </main>
    );
}
