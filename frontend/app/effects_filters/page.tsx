"use client";
import { Box, Button, Text, Collapsible } from "@chakra-ui/react";
import { useEffect, useState,  DragEvent } from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { MdDelete } from "react-icons/md";
import { useController, useForm, FieldError } from "react-hook-form"
import { filters, effects, defaultOpts, OptKey, FilterType } from "../utils/tables"
import { LuChevronRight } from "react-icons/lu"
import { SlidersItems } from "@/components/SlidersItems";
import { z } from "zod"
import { CheckboxesWithHeading } from "@/components/Checkboxes";
import { useFilters, useFiltersParams, useFiltersLogic } from "../utils/context_utils";
import { v4 as uuidv4 } from "uuid";
import { clearFilters, addFilter, deleteFilter, swapFilters } from "../utils/integration_utils";


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
}

function getPosFromFiltered(list1: blockType[], idx: string) {
    console.log("FILTERING", list1.filter(item => item.val !== ""), idx)
    const res:number = list1.filter(item => item.val !== "").findIndex(i => i.id === idx);
    return res;
}

function DraggableList({ attr }: DraggableListProps) {

    const { data, setData } = useFilters();
    const { paramsData, setParamsData } = useFiltersParams();
    const { deleteFilterFromList, addFilterToList, swapFiltersFromList } = useFiltersLogic();

    const myData = data[attr];
    const [listData, setListData] = useState<string[]>(myData);
    const [blocks, setBlocks] = useState<blockType[]>([]);

    useEffect(
        () => {
            setListData(myData ?? []);
            const newArr = Array.from({ length: 3 }, (_, i):blockType => {return { val: "", id:uuidv4() }});
            setBlocks(newArr);
            setParamsData([]);
        }, [myData]);


    const [dragIndex, setDragIndex] = useState<number | null>(null);
    const [dragBlockInd, setDragBlockInd] = useState<number | null>(null);

    const dragStartList = (index: number) => {
        setDragIndex(index);
    };


    const dragStartBlock = (index: number) => {
        setDragBlockInd(index);
    }


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
                deleteFilterFromList(index1);
            }
            newBlocks[index] = {val: draggedItem, id: newID}; // update

            const index2 = getPosFromFiltered(newBlocks, newID) // actual position after adding
            addFilterToList(draggedItem as FilterType, Object.keys(defaultOpts) as OptKey[], index2);

            (async () => {
                if (blocks[index].val !== "") {
                    await deleteFilter(index1);
                }
                await addFilter(draggedItem, index2);
            })();

            setDragIndex(null);
        }

        else if (dragBlockInd !== null) {
            const draggedItem = blocks[dragBlockInd];

            const temp = newBlocks[dragBlockInd];
            newBlocks[dragBlockInd] = newBlocks[index];
            newBlocks[index] = temp;

            const index1 = getPosFromFiltered(newBlocks, newBlocks[index].id);
            const index2 = getPosFromFiltered(newBlocks, newBlocks[dragBlockInd].id);

            swapFiltersFromList(index1, index2);

            (async () => { await swapFilters(index1, index2) })(); // swap 2 existing filters

            setDragBlockInd(null);
        }

        setBlocks(newBlocks);
    }


    const handleDelete = (index: number) => {
        const newBlocks = [...blocks];

        const index1 = getPosFromFiltered(newBlocks, newBlocks[index].id); // we search in arr before change
        newBlocks[index].val = "";
        deleteFilterFromList(index1);

        (async () => {
            await deleteFilter(index1)
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
                                        <Box key={index} as="li" color="white" bg="gray.500" rounded="2xl" maxW="30%" shadow="md" p={2}
                                            draggable
                                            onDragStart={() => dragStartList(index)}
                                            cursor="grab"
                                            className={index === dragIndex ? "dragging" : ""} >
                                            {item}
                                        </Box>
                                    )
                                })
                            }
                        </Box>

                        {
                            listData.length > 0 &&
                            <>
                                <Box h="5" />

                                <Box as="ul" display="flex" flexDirection="row" listStyleType="none" p={0} gap={2} >
                                    {
                                        blocks.map((item, index) => {
                                            return (
                                                <Box key={index} as="li" color="white" bg="green.500"
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

                <Box display="flex" justifyContent="center" minW="60%">
                    <Button size="sm" type="submit">
                        Submit
                    </Button>
                </Box>

            </form>

            <Box h="10" />

            <Box minW="80%">
                <DraggableList attr="filters" />
                <Box h="10" />
                <SlidersItems neededItems={filters} attr="filters" idx={0} />
                <Box h="10" />
                <DraggableList attr="effects" />
                <Box h="10" />
                <SlidersItems neededItems={effects} attr="effects" idx={1} />
                <Box h="10" />

            </Box>
        </Box>
    )
}


export default function Home() {
    return ( <Page /> )
}