"use client"

import { ChakraProvider, defaultSystem } from "@chakra-ui/react"
import {
  ColorModeProvider,
  type ColorModeProviderProps,
} from "./color-mode"
import { writeData } from "@/app/sequencer/actions";
import { useEffect } from "react";

export function Provider(props: ColorModeProviderProps) {
  useEffect(() => {
        writeData();
    }, []);
  return (
    <ChakraProvider value={defaultSystem}>
      <ColorModeProvider {...props} />
    </ChakraProvider>
  )
}
