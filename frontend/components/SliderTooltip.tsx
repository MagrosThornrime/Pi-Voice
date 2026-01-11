import { SliderProps } from "./SlidersItems";
import { Flex, Box, Text } from "@chakra-ui/react";

type MyProps = {
    Props: SliderProps;
}
export function SliderTooltip({Props} : MyProps) {
    let dist = Props.bounds[1] - Props.bounds[0];
    
    const val = Props.actValue;
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
                        (dist <= 0.005) ? Number.parseFloat(val.toFixed(4)) :
                            (dist <= 2) ? Number.parseFloat(val.toFixed(2))  :
                                ((dist <= 50) ? Number.parseFloat(val.toFixed(2)) : Number.parseFloat(val.toFixed(2)))
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