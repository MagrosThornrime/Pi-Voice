"use client";

import React, { useState } from "react";
import {
  Box,
  Heading,
  Text,
  Grid,
  Slider
} from "@chakra-ui/react";

export default function Home() {
  return (
    <Box minH="100vh" bg="gray.50" p={10}>
      <Heading textAlign="center" mb={10} color="teal.600">
        Audio Platform
      </Heading>
      <Box h="80" />
      <Grid
        templateColumns={{
          base: "1fr",
          md: "repeat(2, 1fr)",
          lg: "repeat(3, 1fr)",
        }}
        gap={10}
        maxW="800px"
        mx="auto"
      >
        {["Volume","Attack","Sustain","Decay","Release","Filter"].map((i) => (
          <Box key={i} p={5} bg="grey" rounded="2xl" shadow="md">
            <Text mb={2} fontWeight="medium" textAlign="center">
              {i}
            </Text>
            <Slider.Root defaultValue={[40]}>
              <Slider.Control>
                <Slider.Track>
                  <Slider.Range />
                </Slider.Track>
                <Slider.Thumbs />
              </Slider.Control>
            </Slider.Root>
          </Box>
        ))}
      </Grid>
    </Box>
  );
}
