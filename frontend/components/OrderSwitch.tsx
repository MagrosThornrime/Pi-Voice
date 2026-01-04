import { Switch, Text, Box } from "@chakra-ui/react"
import { HiCheck, HiX } from "react-icons/hi" // in order to memorize previously used icon
import { FaAngleUp, FaAnglesUp } from "react-icons/fa6";

type OrderSwitchProps = {
    label:string;
}

export function OrderSwitch(props:OrderSwitchProps){
  return (
    <>
        <Text fontWeight="medium" fontSize = "lg" >{props.label}</Text>
        <Box h = "2"/>

        <Switch.Root size="lg">
            <Switch.HiddenInput />
            <Switch.Control>
            <Switch.Thumb>
                <Switch.ThumbIndicator fallback={<FaAngleUp color = "blue" />}>
                <FaAnglesUp color = "green" />
                </Switch.ThumbIndicator>
            </Switch.Thumb>
            </Switch.Control>
        </Switch.Root>
    </>
  )
}