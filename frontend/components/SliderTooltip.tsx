import { SliderProps } from "./SlidersItems";
import { Flex, Box, Text } from "@chakra-ui/react";

type MyProps = {
    Props: SliderProps;
}
export function SliderTooltip({Props} : MyProps) {
    return (
        <Flex w="100%" mb={2} alignItems="center">
            <Box flex="1">
                <Text textAlign="left">
                    {Props.bounds[0]}
                </Text>
            </Box>

            <Box flex="1">
                <Text textAlign="center" fontWeight="bold">
                    Val: {Math.round(Props.actValue)}
                </Text>
            </Box>

            {/* Right: Takes up 1/3 space, text aligned right */}
            <Box flex="1">
                <Text textAlign="right">
                    {Props.bounds[1]}
                </Text>
            </Box>
        </Flex>
    )
}