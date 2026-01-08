"use client";

import { useEffect, useRef, useState } from "react";

import {
  Box,
  Stack,
  Button,
  Grid,
  GridItem,
  Editable, IconButton
} from "@chakra-ui/react";
import { LuPencilLine } from "react-icons/lu"
import { MdDelete } from "react-icons/md";

function getRandomColor() {
  var letters = '0123456789ABCDEF';
  var color = '#';
  for (var i = 0; i < 6; i++) {
    color += letters[Math.floor(Math.random() * 16)];
  }
  return color;
}

export default function Page() {

    const [isPlaying, setIsPlaying] = useState<boolean>(() => {
        try {
            return sessionStorage.getItem("seq_playing") === "Stop";
        } catch {
            return false;
        }
    });

    const [isRecording, setIsRecording] = useState<boolean>(() => {
        try{
            return sessionStorage.getItem("seq_recording") === "Stop recording";
        } catch{
            return false;
        }
    })

    const [sounds, setSounds] = useState<string[]>([]);
    const [colorMap, setColorMap] = useState<Record<string, string>>({});
    const [dragIndex, setDragIndex] = useState<number | null>(null);

    function generate_name(){
        let last = 1;
        while (sounds.includes("Dzwiek" + last)) last++;
        return "Dzwiek" + last;
    }

    const isFirstRender = useRef(0);

    useEffect(() => {
        const saved = sessionStorage.getItem("sounds");
        if (saved){
            setSounds(JSON.parse(saved));
            const saved2 = sessionStorage.getItem("colorMap");
            if (saved2) setColorMap(JSON.parse(saved2));
        }
    }, []);

    useEffect(() => {
        if (isFirstRender.current<1){
            isFirstRender.current = isFirstRender.current+1;
        }else{
            sessionStorage.setItem("sounds", JSON.stringify(sounds));
        }
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
        if (isFirstRender.current>1){
            sessionStorage.setItem("colorMap", JSON.stringify(colorMap));
        }
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

        await window.synthAPI.sequencerMoveSample(dragIndex,index);
    };

    return (
        <Box minH="100vh" bg="gray.50" p={10}>
            <Stack>
                <Button
                    disabled={isPlaying}
                    bg={(() => {
                        try {
                            return (sessionStorage.getItem("seq_recording") === "Stop recording") ? "red.400" : "green.400";
                        } catch {
                            return "green.400";
                        }
                    })()}

                    onClick={async () => {
                        try {
                            if (sessionStorage.getItem("seq_recording")=="Stop recording"){
                                setIsRecording(false);
                                sessionStorage.setItem("seq_recording","Record to sequencer");
                                await window.synthAPI.sequencerStopRecording();
                                setSounds(prev => [...prev, generate_name()]);
                            }else{
                                setIsRecording(true);
                                sessionStorage.setItem("seq_recording","Stop recording");
                                await window.synthAPI.sequencerStartRecording(44100,2,40);
                            }
                        } catch (err) {
                            console.error(err);
                        }
                    }}
                >
                    {
                        (() => {
                            try {
                                return sessionStorage.getItem("seq_recording") ?? "Record to sequencer";
                            } catch {
                                return "Record to sequencer";
                            }
                        })()
                    }
                </Button>
                <Box h="10"/>
                <Grid
                    w="100%"
                    templateColumns={{lg:"repeat(4, 1fr)", xl:"repeat(8, 1fr)"}}
                    gap={4}
                >
                    {
                        sounds.map((item, index) => (
                             <GridItem colSpan={1} key={item}>
                                <Box
                                    minHeight="60px" minWidth="13%" shadow="md" p={2}
                                    display="flex" alignItems="center"
                                    justifyContent={{lg:"flex-start", xl:"flex-start"}}
                                    position="relative"
                                    bg={colorMap[item]}
                                    draggable={!isPlaying}
                                    onDragStart={() => {
                                        if(isPlaying)return;
                                        handleDragStart(index);
                                    }}
                                    onDragOver={(e) => {
                                        if(isPlaying)return;
                                        handleDragOver(e);
                                    }}
                                    onDrop={() => {
                                        if(isPlaying)return;
                                        handleDrop(index);
                                    }}
                                    cursor={!isPlaying ? "grab" : "not-allowed"}
                                    opacity={dragIndex === index ? 0.4 : 1}
                                >
                                    <Button size={{lg:"sm", xl:"lg"}}
                                        p={1}
                                        minW={0}
                                        bg="transparent"
                                        position="absolute"
                                        left="8px"
                                        _hover={{ bg: "red.600" }}
                                        _active={{ bg: "red.700" }}
                                        disabled={isPlaying}
                                        onClick={() => {
                                            setSounds(prev => prev.filter(s => s !== item));
                                            window.synthAPI.sequencerRemoveSample(index);
                                        }
                                    }>
                                        <MdDelete/>
                                    </Button>

                                    <Box position="relative" display="flex" alignItems="center" w="80%" ml="auto">
                                        <Editable.Root
                                            defaultValue={item}
                                            fontSize={{lg: "sm", xl: "xl" }}
                                            ml={{ xl: 1, base: 2 }}
                                            flex="1"
                                        >
                                            <Editable.Preview pointerEvents="none" />
                                            <Editable.Input />

                                            <Editable.Control>
                                                <Editable.EditTrigger asChild>
                                                    <IconButton p = {3}
                                                        position="absolute"
                                                        right={{sm: "8px", lg:"3px", xl:"1px"}}
                                                        variant="ghost"
                                                        size="xs"
                                                        aria-label="Edit"
                                                    >
                                                        <LuPencilLine color="white" />
                                                    </IconButton>
                                                </Editable.EditTrigger>
                                            </Editable.Control>
                                        </Editable.Root>
                                    </Box>

                                </Box>
                            </GridItem>
                        ))
                    }
                </Grid>
                <Box h="10"/>
                <Button
                    disabled={isRecording||sounds.length==0}
                    bg={(() => {
                        try {
                            return (sessionStorage.getItem("seq_playing") === "Stop") ?  "red.400" : "green.400";
                        } catch { return "green.400";
                            }
                    })()}

                    onClick={async () => {
                        try {
                            if (sessionStorage.getItem("seq_playing") === "Stop") {
                                setIsPlaying(false);
                                sessionStorage.setItem("seq_playing","Play");
                                await window.synthAPI.sequencerDeactivate();                                
                            } else {
                                setIsPlaying(true);
                                sessionStorage.setItem("seq_playing","Stop");
                                await window.synthAPI.sequencerActivate();
                            }
                            console.log(sessionStorage.getItem("seq_playing"));
                        } catch (err) {
                            console.error(err);
                        }
                    }}
                >
                    {
                        (() => {
                            try {
                                return sessionStorage.getItem("seq_playing") ?? "Play";
                            } catch {
                                return "Play";
                            }
                        })()
                    }
                    
                </Button>
            </Stack>
        </Box>
    );
}