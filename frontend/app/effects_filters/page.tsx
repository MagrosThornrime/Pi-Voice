"use client";
import { Box, Button, Text, Collapsible, Stack } from "@chakra-ui/react";
import { useEffect, useState,  DragEvent, useRef } from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { MdDelete } from "react-icons/md";
import { useController, useForm } from "react-hook-form"
import { filters, effects, defaultOpts, OptKey, FilterType, EffectType, defaultEffectOpts, OptEffectKey} from "../utils/tables"
import { LuChevronRight } from "react-icons/lu"
import { SlidersItems } from "@/components/SlidersItems";
import { z } from "zod"
import { usePreset } from "@/components/ui/presetsProvider";
import { CheckboxesWithHeading } from "@/components/Checkboxes";
import { useFilters, useFiltersParams, useFiltersLogic } from "../utils/context_utils";
import { v4 as uuidv4 } from "uuid";
import { clearFilters, addFilter, deleteItem, swapItems, moveItem, addEffect } from "../utils/integration_utils";

const FIELDS = 4

const FiltersFormSchema = z.object({
    filters: z.array(z.string()).max(3, {
        message: "You cannot select more than 3 filters.",
    }),
    effects: z.array(z.string()).max(3, {
        message: "You cannot select more than 3 effects.",
    })
})

export type FiltersData = z.infer<typeof FiltersFormSchema>


type DraggableListProps = {
    attr: "filters" | "effects";
}

type blockType = {
    val: string;
    id: string;
    group: "filters" | "effects" | "empty"
}


type listType = {
    val: string;
    group: "filters" | "effects";
}

const groupColor:Record<string, string> = {
    "filters" : "green.500",
    "effects" : "purple.600",
    "empty" : "blue.500"
}

function getPosFromFiltered(list1: blockType[], idx: string) {
    const res:number = list1.filter(item => item.val !== "").findIndex(i => i.id === idx);
    return res;
}

function getListFromData(data:FiltersData):listType[] {
    const dataFilters = data.filters.map((x, _):listType => {return {val: x, group: "filters"}});
    const dataEffects = data.effects.map((x, _):listType => {return {val: x, group: "effects"}});
    return dataFilters.concat(dataEffects);
}

function DraggableList({ attr }: DraggableListProps) {

    const { data, setData } = useFilters();
    const { paramsData, setParamsData } = useFiltersParams();
    const { deleteItemFromList, addFilterToList, swapItemsFromList, moveItemInList, addEffectToList } = useFiltersLogic();

    const [listData, setListData] = useState<listType[]>(getListFromData(data));
    const [blocks, setBlocks] = useState<blockType[]>([]);

    const {
        presetNr,
        presetProperties,
        setPresetProperties,
    } = usePreset();

    useEffect(
        () => {
            setListData(getListFromData(data) ?? []);
            if(presetProperties.filters){
                let newArr = Array.from({ length: presetProperties.filters.length },
                     (_, i):blockType => {return { val: presetProperties.filters[i].params.value, id:uuidv4(), group: presetProperties.filters[i].params.group}});

                newArr = newArr.concat(Array.from({ length: FIELDS-newArr.length}, (_, i):blockType => {return { val: "", id:uuidv4(), group: "empty" }}));
                setBlocks(newArr);
                setParamsData(presetProperties.filters);
            }else{
                const newArr = Array.from({ length: FIELDS }, (_, i):blockType => {return { val: "", id:uuidv4(), group: "empty" }});

                setBlocks(newArr);
                setListData(getListFromData(data));
                setParamsData([]);
            }
        }, [data]);
    
    const isFirstRender = useRef(0);
    
    useEffect(
        () => {
            if (isFirstRender.current<2){
                isFirstRender.current = isFirstRender.current+1;
            }else{
                setPresetProperties(prev => ({...prev, filters: paramsData}));
            }
        }, [paramsData]);

    useEffect(() => {
        if(presetProperties.filters!=paramsData){
            let newArr = Array.from({ length: presetProperties.filters.length },
                 (_, i):blockType => {return { val: presetProperties.filters[i].params.value, id:uuidv4(), group: presetProperties.filters[i].params.group }});
            newArr = newArr.concat(Array.from({ length: FIELDS-newArr.length}, (_, i):blockType => {return { val: "", id:uuidv4(), group: "empty" }}));

            setBlocks(newArr);
            setParamsData(presetProperties.filters);
        }
    }, [presetProperties.filters.map(f => f.params.value).join("|")]);

    const [dragIndex, setDragIndex] = useState<number | null>(null);
    const [dragBlockInd, setDragBlockInd] = useState<number | null>(null);

    const dragStartList = (index: number) => {
        setDragIndex(index);
    };


    const dragStartBlock = (index: number) => {
        setDragBlockInd(index);
    };


    const handleDragOver = (e: DragEvent<HTMLDivElement>) => {
        e.preventDefault();
    };


    const handleDrop = (index: number) => {
        if (dragIndex === null && dragBlockInd === null) { return }
        const newBlocks = [...blocks];

        if (dragIndex !== null) { // we drop filter from list to the blocks array
            const draggedItem = listData[dragIndex];
            const newID:string = uuidv4();
        
            let index1:number = -1;
            
            if (newBlocks[index].val !== "") {
                index1 = getPosFromFiltered(newBlocks, newBlocks[index].id); // element that is there before change
                deleteItemFromList(index1);
            }
            newBlocks[index] = {val: draggedItem.val, id: newID, group:draggedItem.group}; // update

            const index2 = getPosFromFiltered(newBlocks, newID) // actual position after adding

            if (newBlocks[index].group == "filters"){
                addFilterToList(draggedItem.val as FilterType, Object.keys(defaultOpts) as OptKey[], index2);
            }
            else{
                const item1 = draggedItem.val as EffectType
                addEffectToList(item1, Object.keys(defaultEffectOpts[item1]) as OptEffectKey[], index2)
            }

            (async () => {
                if (blocks[index].val !== "") {
                    await deleteItem(index1);
                }
                if (newBlocks[index].group == "filters"){
                    await addFilter(draggedItem.val as FilterType, index2);
                }
                else{
                    await addEffect(draggedItem.val as EffectType, index2); // not implemented yet
                }
            })();

            setDragIndex(null);
        }

        else if (dragBlockInd !== null) {
            const draggedItem = blocks[dragBlockInd];

            const temp = newBlocks[dragBlockInd];

            const condDrag:boolean = newBlocks[dragBlockInd].val !== "";
            const condInd: boolean = newBlocks[index].val !== ""

            if (condDrag || condInd){
                const index1 = getPosFromFiltered(newBlocks, newBlocks[index].id);
                const index2 = getPosFromFiltered(newBlocks, newBlocks[dragBlockInd].id);

                if (condDrag && condInd){
                    swapItemsFromList(index1, index2);
                    (async () => { await swapItems(index1, index2) })(); // swap 2 existing filters
                }
                else if (condDrag){
                    const index3 = newBlocks.slice(0, index).filter(item => item.val !== "").length;
                    if (index2 + 1 != index3){ // we insert before index3
                        moveItemInList(index2, index3);
                        (async () => {
                            await moveItem(index2, index3);
                        })();
                    }
                }
                else if (condInd){
                    const index3 = newBlocks.slice(0, dragBlockInd).filter(item => item.val !== "").length;
                    if (index1 + 1 != index3){ // we insert before index3
                        moveItemInList(index1, index3);
                        (async () => {
                            await moveItem(index1, index3);
                        })();
                    }
                }
            }
            newBlocks[dragBlockInd] = newBlocks[index];
            newBlocks[index] = temp;

            setDragBlockInd(null);
        }

        setBlocks(newBlocks);
    }


    const handleDelete = (index: number) => {
        const newBlocks = [...blocks];

        const index1 = getPosFromFiltered(newBlocks, newBlocks[index].id); // we search in arr before change

        newBlocks[index].val = "";
        newBlocks[index].group = "empty";

        deleteItemFromList(index1);

        (async () => {
            await deleteItem(index1)
        })();

        setBlocks(newBlocks);
    }


    return (
        <Collapsible.Root justifyItems={"left"}>

            <Collapsible.Trigger paddingY="3" display="flex" gap="2" alignItems="center" justifyItems={"center"}>

                <Collapsible.Indicator
                    transition="transform 0.2s"
                    _open={{ transform: "rotate(90deg)" }} >
                    <LuChevronRight size={24} color="black" />
                </Collapsible.Indicator>

                <Box maxW="100%">
                    <Text textStyle="2xl" mb={2} color="teal.600" fontWeight="semibold" textAlign="center"> Toggle {attr} order selection </Text>
                </Box>

            </Collapsible.Trigger>

            <Collapsible.Content maxW="100%" minW="100%">

                <Box>
                    <>
                        <Box
                            as="ul"
                            display="flex"
                            flexDirection="row"
                            listStyleType="none"
                            p={0}
                            gap={2}
                        >
                            {
                                listData.map((item, index) => {
                                    return (
                                        <Box key={index} as="li" color="white" bg={ (item.group === "filters") ? "gray.500" : "green.600"} rounded="2xl" maxW="30%" shadow="md" p={2}
                                            draggable
                                            onDragStart={() => dragStartList(index)}
                                            cursor="grab"
                                            className={index === dragIndex ? "dragging" : ""} >
                                            {item.val}
                                        </Box>
                                    )
                                })
                            }
                        </Box>

                        {
                            <>
                                <Box h="5" />

                                <Box as="ul" display="flex" flexDirection="row" listStyleType="none" p={0} gap={2} >
                                    {
                                        blocks.map((item, index) => {
                                            return (
                                                <Box key={index} as="li" color="white" bg={groupColor[item.group]}
                                                    rounded="2xl" minHeight="40px" minWidth="7%" shadow="md" p={2}

                                                    draggable

                                                    onDragStart={() => {
                                                        if (blocks[index].val == "") { return; }
                                                        dragStartBlock(index);
                                                    }}

                                                    onDragOver={handleDragOver}

                                                    onDrop={() => handleDrop(index)} >

                                                    <Button size="xs"
                                                        p={1}
                                                        minW={0}
                                                        bg="transparent"
                                                        _hover={{ bg: "red.600" }}
                                                        _active={{ bg: "red.700" }}

                                                        onClick={(e) => {
                                                            e.stopPropagation;
                                                            handleDelete(index);
                                                        }}
                                                    >

                                                        <MdDelete />

                                                    </Button>

                                                    {item.val}

                                                </Box>
                                            )
                                        })
                                    }
                                </Box>
                            </>
                        }

                    </>
                </Box>

            </Collapsible.Content>

        </Collapsible.Root>
    )
}


function Page() {

    const { handleSubmit, control, formState: { errors } } = useForm<FiltersData>({
        resolver: standardSchemaResolver(FiltersFormSchema),
        defaultValues: {
            filters: [],
            effects: [],
        }
    })


    const filtersField = useController({ control, name: "filters" });
    const effectsField = useController({ control, name: "effects" });

    const { data, setData } = useFilters();

    const { paramsData} = useFiltersParams();

    const invalid = !!errors.filters
    const invalid_eff = !!errors.effects

    return (
        <Box minH="100vh" bg="gray.50" p={10} justifyItems={"left"} alignItems="center">
            <form
                onSubmit={handleSubmit(async (formData) => {
                    setData(formData);

                    await clearFilters(paramsData.length);

                    console.log("SUBMITTED", formData);
                })}
            >
                <Stack direction={"row"} gap={40}>
                    <CheckboxesWithHeading field={filtersField.field}
                        formItems={filters}
                        invalid={invalid}
                        error={Array.isArray(errors.filters)
                            ? errors.filters[0]
                            : errors.filters
                        }
                        ifButton={false}
                        headerText="Select filters" >
                    </CheckboxesWithHeading>

                    <CheckboxesWithHeading field={effectsField.field}
                        formItems={effects}
                        invalid={invalid_eff}
                        error={Array.isArray(errors.effects)
                            ? errors.effects[0]
                            : errors.effects
                        }
                        ifButton={false}
                        headerText="Select effects" >

                    </CheckboxesWithHeading>
                </Stack>

                <Box display="flex" justifyContent="center" minW="60%">
                    <Button size="2xl" type="submit">
                        Submit
                    </Button>
                </Box>

            </form>

            <Box h="10" />

            <Box minW="80%">
                <DraggableList attr="filters" />
                <Box h="10" />
                <SlidersItems attr="filters" />
                <Box h="10" />

            </Box>
        </Box>
    )
}


export default function Home() {
    return ( <Page /> )
}