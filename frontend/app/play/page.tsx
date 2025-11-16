"use client";

import { useState } from "react";

// ---- Custom Error Types ----
export class DirectoryAccessError extends Error {
    constructor(message: string = "Cannot write to the requested directory.") {
        super(message);
        this.name = "DirectoryAccessError";
    }
}

export class ValueOutOfRangeError extends Error {
    constructor(message: string = "Value is out of allowed bounds.") {
        super(message);
        this.name = "ValueOutOfRangeError";
    }
}

// ---- extend synthAPI interface ----
declare global {
    interface Window {
        synthAPI: {
            ports: () => Promise<string[]>;
            openPort: (port: number) => Promise<void>;
            setAmplitude: (value: number) => Promise<void>;
            startRecording: () => Promise<void>;
            stopRecording: () => Promise<void>;

            // New error-related APIs
            setRecordingPath?: (path: string) => Promise<void | DirectoryAccessError>;
            validateValue?: (value: number) => Promise<void | ValueOutOfRangeError>;
        };
    }
}

export default function PlayPage() {
    const [status, setStatus] = useState<string>("idle");
    const [midiPorts, setMidiPorts] = useState<string[]>([]);
    const [selectedPort, setSelectedPort] = useState<number | null>(null);

    // new state for directory input
    const [recordingDir, setRecordingDir] = useState<string>("");

    // --------------------
    // MIDI port handling
    // --------------------
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

    // --------------------
    // Recording directory
    // --------------------
    const applyDirectory = async () => {

        try {
            const result = await window.synthAPI.setRecordingPath(recordingDir);
            if (result instanceof DirectoryAccessError) {
                setStatus(`Error: ${result.message}`);
            } else {
                setStatus(`Directory set to: ${recordingDir}`);
            }
        } catch (err: any) {
            setStatus("Error: " + err.message);
        }
    };

    // --------------------
    // Recording controls
    // --------------------
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
            <h1>Simple Synth Controller</h1>
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

            {/* Recording Directory */}
            <div style={{ marginTop: "20px" }}>
                <h3>Recording Directory</h3>
                <input
                    type="text"
                    placeholder="output"
                    value={recordingDir}
                    onChange={(e) => setRecordingDir(e.target.value)}
                    style={{ padding: "6px", width: "200px" }}
                />
                <button
                    onClick={applyDirectory}
                    style={{ marginLeft: "10px" }}
                >
                    Apply
                </button>
            </div>

            {/* Recording Controls */}
            <div style={{ marginTop: "20px" }}>
                <button onClick={startRecording}>Start Recording</button>
                <button onClick={stopRecording} style={{ marginLeft: "10px" }}>
                    Stop Recording
                </button>
            </div>
        </main>
    );
}
