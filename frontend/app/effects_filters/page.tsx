"use client";
import { Box, Checkbox, Button, Fieldset, Stack, Text, CheckboxGroup, Heading, Collapsible, Flex, Grid, Slider } from "@chakra-ui/react";
import { useContext, ReactNode, useEffect, createContext, useState, Fragment} from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { useController, ControllerRenderProps, useForm, FieldError } from "react-hook-form"
import { LuChevronRight } from "react-icons/lu"
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

type MyItems = {
    label: string;
    value: string;
    opts: string[];
};

const items:MyItems[] = [
    { label: "AllPass", value: "allpass", opts: ["a", "aa"] },
    { label: "BandPass", value: "bandpass", opts: ["b", "bb"] },
    { label: "ButterWorth", value: "butterworth", opts: ["c", "cc"] },
    { label: "HighPass", value: "highpass", opts: ["d", "dd"] },
    { label: "HighShelf", value: "highshelf", opts: ["e", "ee"] },
    { label: "LowPass", value: "lowpass", opts: ["f", "ff"] },
    { label: "LowShelf", value: "lowshelf", opts: ["g", "gg"] },
    { label: "Notch", value: "notch", opts: ["h", "hh"] },
]


const effects:MyItems[] = [
    { label: "aaa", value: "aaa", opts: ["a", "aa"] },
    { label: "bbb", value: "bbb", opts: ["b", "bb"] },
    { label: "ccc", value: "ccc", opts: ["c", "cc"] },
    { label: "ddd", value: "ddd", opts: ["d", "dd"] },
    { label: "eee", value: "eee", opts: ["e", "ee"] },
    { label: "fff", value: "fff", opts: ["f", "ff"] },
    { label: "ggg", value: "ggg", opts: ["g", "gg"] },
    { label: "hhh", value: "hhh", opts: ["h", "hh"] }
]




type FormWithHeadingProps = {
    formItems: { label: string; value: string }[];
    field: ControllerRenderProps<any, any>;
    ifButton: boolean;
    headerText: string;
    buttonText?: string;
    error?: FieldError;
    invalid?: boolean;
};


const buildInitialState = () => {
    const state: Record<string, any> = {};

    items.forEach(item => {
        item.opts.forEach(opt => {
            state[`${item.value}.${opt}`] = [30]
        });
    });

    return state;
};


const buildInitialEndState = () => {
    const state: Record<string, any> = {};

    items.forEach(item => {
        item.opts.forEach(opt => {
            state[`${item.value}.${opt}_end`] = [30]
        });
    });

    return state;
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


type SlidersItemsProps = {
    neededItems: MyItems[];
    attr: "filters" | "effects";
};


function SlidersItems({ neededItems, attr }: SlidersItemsProps) {

    const { data } = useFilters();

    const filteredItems = neededItems.filter(item =>
        data[attr].includes(item.value)
    );

    const [Values, setValues] = useState<Record<string, any>>(buildInitialState);
    const [EndValues, setEndValues] = useState<Record<string, any>>(buildInitialEndState);

    const setSliderValue = (itemValue: string, opt: string, newValue: number) => {
        setValues(prev => ({
            ...prev,
            [`${itemValue}.${opt}`]: newValue
        }));
    };

    const setEndSliderValue = (itemValue: string, opt: string, newValue: number) => {
        setEndValues(prev => ({
            ...prev,
            [`${itemValue}.${opt}`]: newValue
        }));
    };

    const sliders = filteredItems.flatMap(obj => {
        return (
            <Fragment key={obj.value}>
                <Box p={5} bg="grey" rounded="2xl" maxW="100%" shadow="md">
                    <Text mb={2} fontWeight="medium" textAlign="center">{obj.value}</Text>
                    {
                        obj.opts.map(opt => {
                            const key = `${obj.value}.${opt}`;
                            const Value = Values[key] ?? 0; // upewniamy się, że to liczba
                            return (
                                <Fragment key={`${obj.value}.${opt}`}>
                                    <Slider.Root
                                        key={key}
                                        value={[Value]} // slider wymaga tablicy

                                        onValueChange={(details) => {
                                            setSliderValue(obj.value, opt, details.value[0])
                                            console.log("SLIDER VALUE: ", Values[key])
                                        }}

                                        onValueChangeEnd={(details) => {
                                            setEndSliderValue(obj.value, `${opt}_end`, details.value[0]);
                                            console.log("SLIDER END VALUE: ", EndValues[`${key}_end`])
                                            // integration with backend will be here
                                        }} >

                                        <Slider.Label color="white"> {`${opt}`} </Slider.Label>
                                        <Slider.Control>
                                            <Slider.Track>
                                                <Slider.Range />
                                            </Slider.Track>
                                            <Slider.Thumbs />
                                        </Slider.Control>
                                    </Slider.Root>

                                    <Stack mt="3" gap="1">
                                        <Text> Wartość: <b>{Math.round(EndValues[`${key}_end`] * 100) / 100}</b> </Text>
                                    </Stack>
                                    <Box h="5" />

                                </Fragment>
                            );
                        })
                    }
                </Box>
                {/* <Box h="10" /> */}
            </Fragment>
        )
    });

    return (
        <Box>
            <Collapsible.Root defaultOpen justifyItems={"center"}>

                <Collapsible.Trigger paddingY="3" display="flex" gap="2" alignItems="center" justifyItems={"center"}>

                    <Collapsible.Indicator
                        transition="transform 0.2s"
                        _open={{ transform: "rotate(90deg)" }} >
                        <LuChevronRight size={24} color="black" />
                    </Collapsible.Indicator>

                    <Box maxW="100%">
                        <Text textStyle="2xl" mb={2} color="teal.600" fontWeight="semibold" textAlign="center"> Toggle {attr} </Text>
                    </Box>

                </Collapsible.Trigger>

                <Collapsible.Content maxW="100%" minW="70%">

                    <Grid templateColumns={{ base: "1fr", md: "repeat(3, 1fr)", lg: "repeat(3, 1fr)", }} gap={10} maxW="1000px" mx="auto">
                        {sliders}
                    </Grid>

                </Collapsible.Content>

            </Collapsible.Root>

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
            <form onSubmit={
                handleSubmit((formData) => {
                    console.log("SUBMITTED", formData);
                    setData(formData);
                }
                )
            } >
                <Stack direction="row" gap={40} >
                    <CheckboxesWithHeading field={filtersField.field}
                        formItems={items}
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
                        {/* buttonText="HAHAHA" */}

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
                <SlidersItems neededItems = {items} attr = "filters"/>
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