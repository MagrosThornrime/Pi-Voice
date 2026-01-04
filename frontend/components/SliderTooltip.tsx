import { SliderProps } from "./SlidersItems";
import { Flex, Box, Text } from "@chakra-ui/react";

type MyProps = {
    Props: SliderProps;
}
export function SliderTooltip({Props} : MyProps) {
    return (
        <Flex w="100%" mb={2} alignItems="center">
            <Box flex="1">
                <Text textAlign="left" fontWeight="bold">
                    {Props.bounds[0]}
                </Text>
            </Box>

            <Box flex="1">
                <Text textAlign="center" fontWeight="bold">
                    Val: 
                    {
                        (Props.bounds[1] - Props.bounds[0] < 0.01) ? Math.round(Props.actValue * 10000) / 10000 :
                            (Props.bounds[1] - Props.bounds[0] < 2) ? Math.round(Props.actValue * 100) / 100 :
                                ((Props.bounds[1] - Props.bounds[0] < 10) ? Math.round(Props.actValue * 10) / 10 : Math.round(Props.actValue))
                    }
                </Text>
            </Box>

            <Box flex="1">
                <Text textAlign="right"fontWeight="bold">
                    {Props.bounds[1]}
                </Text>
            </Box>
        </Flex>
    )
}