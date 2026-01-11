import { Switch, Text, Box } from "@chakra-ui/react"
import { HiCheck, HiX } from "react-icons/hi" // in order to memorize previously used icon
import { FaAngleUp, FaAnglesUp } from "react-icons/fa6";
import { useState } from "react";
import { setFilterParam } from "@/app/utils/integration_utils";
import { FilterType, OptKey } from "@/app/utils/tables";
import { ItemsParams } from "@/app/utils/context_utils";

type OrderSwitchProps = {
    label:OptKey;
    filterName: FilterType;
    itemID: string;
    paramsData: ItemsParams[];
}

export function OrderSwitch({label, itemID, paramsData, filterName}:OrderSwitchProps){
  const [checked, setChecked] = useState<boolean>(false);
  return (
    <>
        <Text fontWeight="medium" fontSize = "lg" >{label}</Text>
        <Box h = "2"/>

        <Switch.Root size="lg" checked={checked} 
        onCheckedChange={async (details) => {
          await setFilterParam(filterName, paramsData.findIndex((f) => f.id === itemID), 0, details.checked ? 2 : 1);
          setChecked(details.checked);
          }}>
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