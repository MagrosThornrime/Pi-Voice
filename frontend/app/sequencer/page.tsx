"use client";

import { useEffect, useState } from "react";

import {
  Box,
  Stack,
  Button,
  Grid,
  GridItem,
} from "@chakra-ui/react";

let last : number = 0;
function generate_name(){
    last+=1;
    return("Dzwiek"+String(last/2))
}
function getRandomColor() {
  var letters = '0123456789ABCDEF';
  var color = '#';
  for (var i = 0; i < 6; i++) {
    color += letters[Math.floor(Math.random() * 16)];
  }
  return color;
}

export default function Page() {
    const [sounds, setSounds] = useState<string[]>(() => {
        const saved = sessionStorage.getItem("sounds");
        return saved ? JSON.parse(saved) : [];
    });
    const [colorMap, setColorMap] = useState<Record<string, string>>(() => {
        const saved = sessionStorage.getItem("colorMap");
        return saved ? JSON.parse(saved) : {};
    });
    const [dragIndex, setDragIndex] = useState<number | null>(null);
    const [buttonText, setButtonText] = useState<string>("Record to sequencer");
    const [buttonText2, setButtonText2] = useState<string>("Play");

    useEffect(() => {
        sessionStorage.setItem("sounds", JSON.stringify(sounds));
        setColorMap(prev => {
            const copy: Record<string, string> = { ...prev };
            sounds.forEach(s => {
            if (!copy[s]) {
                copy[s] = getRandomColor();
            }
            });
            return copy;
        });
    }, [sounds]);

    useEffect(() => {
        sessionStorage.setItem("colorMap", JSON.stringify(colorMap));
    }, [colorMap]);

    const handleDragStart = (index: number) => {
        setDragIndex(index);
    };

    const handleDragOver = (e: React.DragEvent<HTMLDivElement>) => {
        e.preventDefault();
    };
    
    const handleDrop = async (index: number) => {
        if (dragIndex === null || dragIndex === index){setDragIndex(null); return;}

        const newArr = [...sounds];

        const [movedItem] = newArr.splice(dragIndex, 1);
        newArr.splice(index, 0, movedItem);

        setSounds(newArr);
        setDragIndex(null);

        // await window.synthAPI.sequencerMoveSample(dragIndex,index); //TODO Krystian weź to zdebuguj
    };

    return (
        <Box minH="100vh" bg="gray.50" p={10}>
            <Stack>
                <Button
                    bg={buttonText === "Record to sequencer" ? "green.400" : "red.400"}
                    onClick={async () => {
                        try {
                            if (buttonText === "Record to sequencer") {
                                await window.synthAPI.sequencerStartRecording(44100,2,40);
                                setButtonText("Stop recording");
                            } else {
                                await window.synthAPI.sequencerStopRecording();
                                setButtonText("Record to sequencer");
                                setSounds(prev => [...prev, generate_name()]);
                            }
                        } catch (err) {
                            console.error(err);
                        }
                    }}
                >
                    {buttonText}
                </Button>
                <Box h="10"/>
                <Grid
                    w="80%"
                    templateColumns="repeat(8, 1fr)"
                    gap={4}
                >
                    {
                        sounds.map((item, index) => (
                             <GridItem colSpan={1} key={item}>
                                <Box
                                    draggable
                                    onDragStart={() => handleDragStart(index)}
                                    onDragOver={handleDragOver}
                                    onDrop={() => handleDrop(index)}
                                    cursor="grab"
                                    opacity={dragIndex === index ? 0.4 : 1}
                                >
                                    <Button bg={colorMap[item]} onClick={() => {
                                        setSounds(prev => prev.filter(s => s !== item));
                                        window.synthAPI.sequencerRemoveSample(index);
                                    }}>
                                        Remove {item}
                                    </Button>
                                </Box>
                            </GridItem>
                        ))
                    }
                </Grid>
                <Box h="10"/>
                <Button
                    bg={buttonText2 === "Play" ? "green.400" : "red.400"}
                    onClick={async () => {
                        try {
                            if (buttonText2 === "Play") {
                                await window.synthAPI.sequencerActivate();
                                setButtonText2("Stop");
                            } else {
                                await window.synthAPI.sequencerDeactivate();
                                setButtonText2("Play");
                            }
                        } catch (err) {
                            console.error(err);
                        }
                    }}
                >
                    {buttonText2}
                </Button>
            </Stack>
        </Box>
    );
}