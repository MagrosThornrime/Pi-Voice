"use client";

import { useState } from "react";

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

export default function Page() {
    const [sounds, setSounds] = useState<string[]>([]);
    const [dragIndex, setDragIndex] = useState<number | null>(null);
    const [buttonText, setButtonText] = useState<string>("Record to sequencer");
    const [buttonText2, setButtonText2] = useState<string>("Play");

    const handleDragStart = (index: number) => {
        setDragIndex(index);
    };

    const handleDragOver = (e: React.DragEvent<HTMLDivElement>) => {
        e.preventDefault();
    };
    
    const handleDrop = (index: number) => {
        if (dragIndex === null || dragIndex === index){setDragIndex(null); return;}

        const newArr = [...sounds];

        const [movedItem] = newArr.splice(dragIndex, 1);
        newArr.splice(index, 0, movedItem);

        setSounds(newArr);
        setDragIndex(null);
    };

    return (
        <Box minH="100vh" bg="gray.50" p={10}>
            <Stack>
                <Button
                    bg={buttonText === "Record to sequencer" ? "green.400" : "red.400"}
                    onClick={async () => {
                        try {
                            if (buttonText === "Record to sequencer") {
                                setButtonText("Stop recording");
                            } else {
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
                             <GridItem colSpan={1}>
                                <Box
                                    draggable
                                    onDragStart={() => handleDragStart(index)}
                                    onDragOver={handleDragOver}
                                    onDrop={() => handleDrop(index)}
                                    cursor="grab"
                                    opacity={dragIndex === index ? 0.4 : 1}
                                >
                                    <Button bg="red" onClick={() => setSounds(prev => prev.filter(s => s !== item))}>
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
                                setButtonText2("Stop");
                            } else {
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