"use client";

import {
  Box,
  Flex,
  HStack,
  Text,
  IconButton,
  Button,
  Menu,
  Portal
} from "@chakra-ui/react";
import Link from "next/link";
import { LuExternalLink} from "react-icons/lu";
import { MuseoModerno } from "next/font/google";

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
        <Menu.Root>
          <Menu.Trigger paddingY="3" as="div">
            <IconButton
              style={{ fontSize: "1.5rem" }}
              className={museoModerno.className}
              aria-label={"Open Menu"}
              onClick={() => console.log("Cluecked!")}
            >
            Pi-Voice
            </IconButton>
          </Menu.Trigger>
          <Portal>
            <Menu.Positioner>
              <Menu.Content>
                {Links.map((link) => (
                  <Menu.Item key={link.href} asChild value={link.name}>
                    <Link href={link.href} rel="noreferrer">
                      {link.name}
                      {link.external ? <LuExternalLink /> : null}
                    </Link>
                  </Menu.Item>
                ))}
              </Menu.Content>
            </Menu.Positioner>
          </Portal>
        </Menu.Root>

        <HStack alignItems={"center"}>
          <Button variant={"solid"} colorScheme={"teal"} size={"sm"}>
            prev
          </Button>
          <Box>
            <Text
              style={{ fontSize: "1.5rem" }}
            >
              Preset 1
            </Text>
          </Box>
          <Button variant={"solid"} colorScheme={"teal"} size={"sm"} mr={4}>
            next
          </Button>
        </HStack>
        <Box w={10}/>
      </Flex>
    </Box>
  );
}