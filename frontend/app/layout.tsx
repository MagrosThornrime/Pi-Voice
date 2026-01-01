import type { Metadata } from "next";
import { Geist, Geist_Mono } from "next/font/google";
import "./globals.css";
import { Provider } from "@/components/ui/provider"
import { Providers } from "@/components/Providers";
import Header from "@/components/layout/Header";
import { Box, Center, Container } from "@chakra-ui/react"
import { PresetProvider } from "@/components/ui/presetsProvider"

const geistSans = Geist({
  variable: "--font-geist-sans",
  subsets: ["latin"],
});

const geistMono = Geist_Mono({
  variable: "--font-geist-mono",
  subsets: ["latin"],
});

export const metadata: Metadata = {
  title: "Pi-Voice",
  description: "Create your own sounds!",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html suppressHydrationWarning>
      <body>
        <Providers>
          <Provider>
            <PresetProvider>
              <Box p={0} m={0} minH="100vh" w="100vw">
                <Header />
                <Box as="main" w="100%" minH="100dvh" px={10} pt={24} pb={10}>{children}</Box>
              </Box>
            </PresetProvider>
          </Provider>
        </Providers>
      </body>
    </html>
  );
}
