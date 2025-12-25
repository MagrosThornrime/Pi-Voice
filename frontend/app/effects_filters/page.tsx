"use client";
import { Box, Checkbox, Button, Fieldset, Stack, Text, CheckboxGroup, Heading, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import { useContext, ReactNode, useEffect, createContext, useState, Fragment, DragEvent} from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { useController, ControllerRenderProps, useForm, FieldError } from "react-hook-form"
import {filters, effects} from "../utils/tables"
import { SlidersItems } from "@/components/SlidersItems";
import { z } from "zod"


const FiltersFormSchema = z.object({
    filters: z.array(z.string()).max(3, {
        message: "You cannot select more than 3 filters.",
    }),
    effects: z.array(z.string()).max(3, {
        message: "You cannot select more than 3 effects.",
    })
})


type FiltersData = z.infer<typeof FiltersFormSchema>


type FiltersContextType = {
    data: FiltersData;
    setData: (value: FiltersData) => void;
};


const FiltersContext = createContext<FiltersContextType | undefined>(undefined);


export function FiltersProvider({ children }: { children: ReactNode }) {
    const [data, setData] = useState<FiltersData>({
        filters: [],
        effects: []
    });

    return (
        <FiltersContext.Provider value={{ data, setData }}>
            {children}
        </FiltersContext.Provider>
    );
}


export function useFilters() {
    const ctx = useContext(FiltersContext);
    if (!ctx) {
        throw new Error("useFilters must be used inside FiltersProvider");
    }
    return ctx;
}


type FormWithHeadingProps = {
    formItems: { label: string; value: string }[];
    field: ControllerRenderProps<any, any>;
    ifButton: boolean;
    headerText: string;
    buttonText?: string;
    error?: FieldError;
    invalid?: boolean;
};


function CheckboxesWithHeading({
    formItems,
    field,
    ifButton,
    headerText,
    buttonText,
    error,
    invalid,
}: FormWithHeadingProps
) {
    return (
        <Box>
            <Heading size="3xl" textAlign="center" mb={10} color="teal.600">
                {headerText}
            </Heading>

            <Fieldset.Root invalid={invalid}>
                <Fieldset.Legend color={"teal.600"}> </Fieldset.Legend>
                <CheckboxGroup color={"black"}
                    invalid={invalid}
                    value={field.value}
                    onValueChange={(vals: string[]) => field.onChange(vals)}
                    name={field.name}
                >
                    <Fieldset.Content>
                        <Grid templateColumns={{
                            base: "2fr",
                            md: "repeat(4, 1fr)",
                            lg: "repeat(4, 1fr)",
                        }}
                            gap={10} maxW="800px" mx="auto">
                            {
                                formItems.map((item) => (
                                    <Checkbox.Root key={item.value} value={item.value}>
                                        <Checkbox.HiddenInput />
                                        <Checkbox.Control />
                                        <Checkbox.Label>{item.label}</Checkbox.Label>
                                    </Checkbox.Root>
                                ))
                            }

                        </Grid>

                    </Fieldset.Content>

                    <Box h="5" />

                </CheckboxGroup>

                <Box textAlign="center">
                    {
                        error && (
                            <Fieldset.ErrorText>{error.message}</Fieldset.ErrorText>
                        )
                    }
                    <Box h="2" />

                    {
                        ifButton &&
                        <Button size="sm" type="submit" alignSelf="flex-start">
                            {buttonText || "Submit"}
                        </Button>
                    }

                </Box>

            </Fieldset.Root>
        </Box>
    )
}


let currentFilters : string[] = [];

async function clearFilters(filtersNumber: number) {
    for(let i=filtersNumber-1; i>=0; i--){
        await window.synthAPI.pipelineRemove(i);
    }
}

async function addFilters(filtersArr: string[]) {
    for (let [actIndex, actFilter] of filtersArr.entries()) {
        const filterNumber = filters.findIndex(i => i.value === actFilter);
        await window.synthAPI.pipelineAddFilter(filterNumber, actIndex);
    }
}


export async function setFilterParam(filterName: string, param: number, value: number){
    const filterNumber = currentFilters.findIndex(i => i === filterName);
    await window.synthAPI.pipelineSetFilterParam(filterNumber, param, value)
}

type DraggableListProps = {
    attr:"filters" | "effects";
}


function DraggableList({attr}:DraggableListProps){
    const { data, setData } = useFilters();
    const myData = data[attr];
    const [listData, setListData] = useState<string[]>(myData);
    const [dragIndex, setDragIndex] = useState<number | null >(null);


    const handleDragStart = ( index: number) => {
        setDragIndex(index);
    };


    const handleDragOver = (e: DragEvent<HTMLLIElement>) => {
        e.preventDefault();
    };

    const handleDrop = (index:number) => {
        const newList = [...listData];
        if (dragIndex == null){return}
        const draggedItem = newList[dragIndex];
        newList.splice(dragIndex, 1);
        newList.splice(dragIndex, 0, draggedItem);
        setListData(newList);
        setDragIndex(null);
    }
    return (
        <Box as="ul" listStyleType="circle">
        {
            listData.map((item, index) => {
               return( 
               <li
                key = {index}
                draggable
                onDragStart={ () => handleDragStart(index)}
                onDragOver={handleDragOver}
                onDrop={() => handleDrop(index)}
                className={index == dragIndex ? "dragging": ""} >
                    {item}
                </li>
            )
            })
        }
        </Box>
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

    const invalid = !!errors.filters
    const invalid_eff = !!errors.effects


    return (
        <Box minH="100vh" bg="gray.50" p={10} justifyItems={"center"} alignItems="center">
            <form
                onSubmit={handleSubmit(async (formData) => {
                    setData(formData);

                    await clearFilters(currentFilters.length);
                    currentFilters = formData.filters;

                    await addFilters(currentFilters);

                    console.log("SUBMITTED", formData);
                })}
            >

                <Stack direction="row" gap={40} >
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
                    <Button size="sm" type="submit">
                        Submit
                    </Button>
                </Box>

            </form>

            <Box h="10" />

            <Box minW = "80%">
                <DraggableList attr = "filters" />
                <SlidersItems neededItems = {filters} attr = "filters"/>
                <Box h="10" />
                <SlidersItems neededItems = {effects} attr = "effects"/>
            </Box>
        </Box>
    )
}


export default function Home() {
    return (
        <FiltersProvider>
            <Page />
        </FiltersProvider>
    )
}