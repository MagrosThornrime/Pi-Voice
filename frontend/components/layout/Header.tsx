"use client";

import {
  Box,
  Flex,
  HStack,
  Text,
  IconButton,
  Button,
  Drawer,
  Portal,
  VStack
} from "@chakra-ui/react";
import Link from "next/link";
import { LuExternalLink} from "react-icons/lu";
import { MuseoModerno } from "next/font/google";
import { start } from "repl";
import { useState } from "react";

interface Props {
  children: React.ReactNode;
}

interface SimpleLink {
  name: string;
  href: string;
  external: boolean;
}

const Links: readonly SimpleLink[] = [
  {
    name: "Play",
    href: "/play",
    external: false,
  },
  {
    name: "Oscilators",
    href: "/oscilators",
    external: false,
  },
  {
    name: "Adsr",
    href: "/.",
    external: false,
  },
  {
    name: "GitHub",
    href: "https://github.com/MagrosThornrime/Pi-Voice",
    external: true,
  },
];

const museoModerno = MuseoModerno({ subsets: ["latin"] });

export default function WithAction() {
  const [open, setOpen] = useState(false);
  const [presetNr, setPresetNr] = useState(1);
  return (
    <Box
      as="header"
      bg={"#009FB7"}
      px={4}
      position="fixed"
      w="100%"
      zIndex="99"
    >
      <Flex h={16} alignItems={"center"} justifyContent={"space-between"}>
        <Drawer.Root placement={"start"} open={open} onOpenChange={(details) => setOpen(details.open)}>
          <Drawer.Trigger paddingY="3" as="div">
            <IconButton
              style={{ fontSize: "1.5rem" }}
              className={museoModerno.className}
              aria-label={"Open Menu"}
            >
            Pi-Voice
            </IconButton>
          </Drawer.Trigger>
          <Portal>
            <Drawer.Backdrop />
            <Drawer.Positioner>
              <Drawer.Content bg="#004F5D" color="white" p={6}>
              <VStack padding={4} align="stretch">
                {Links.map((link) => (
                  <Link
                    key={link.href}
                    href={link.href}
                    rel={link.external ? "noreferrer" : undefined}
                    target={link.external ? "_blank" : "_self"}
                    onClick={() => setOpen(false)}
                  >
                    <Box
                      as="button"
                      w="100%"
                      p={5}
                      bg="#00B8D9"
                      _hover={{ bg: "#00D1E0", transform: "scale(1.03)" }}
                      borderRadius="xl"
                      transition="all 0.2s"
                      display="flex"
                      alignItems="center"
                      justifyContent="space-between"
                      className={museoModerno.className}
                      fontSize="1.25rem"
                      fontWeight="600"
                    >
                      <Text>{link.name}</Text>
                      {link.external && <LuExternalLink />}
                    </Box>
                  </Link>
                ))}
              </VStack>
            </Drawer.Content>
            </Drawer.Positioner>
          </Portal>
        </Drawer.Root>

        <HStack alignItems={"center"}>
          <Button 
          variant={"solid"} 
          colorScheme={"teal"} 
          size={"sm"}
          onClick={()=>setPresetNr(presetNr-1)}
          >
            prev
          </Button>
          <Box>
            <Text
              style={{ fontSize: "1.5rem" }}
            >
              Preset {presetNr}
            </Text>
          </Box>
          <Button 
          variant={"solid"} 
          colorScheme={"teal"} 
          size={"sm"} 
          mr={4}
          onClick={()=>setPresetNr(presetNr+1)}
          >
            next
          </Button>
        </HStack>
        <Box w={10}/>
      </Flex>
    </Box>
  );
}