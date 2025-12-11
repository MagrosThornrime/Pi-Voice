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


type Opt = {
    mutable: boolean;
    continuous?: boolean;
    logScale?: boolean;
    range: number[];
    step?: number; // log scale will not require step, neither continuous parameters
}


type MyItems = {
    label: string;
    value: string;
    opts: Record<string, Opt> [];
};


let defaultOpts: Record<string, Opt>[] = [
    { order: { mutable: true, continuous: false, logScale: false, range: [0, 1], step: 1 } },
    { cutoff: { mutable: true, continuous: true, logScale: true, range: [10, 20000] } },
    { gainDB: {mutable: true, continuous: true, logScale: false, range: [-24, 24] } },
    { quality: {mutable: true, continuous: true, logScale: false, range: [0.1, 20.0] } }
    // { sampleRate: { mutable: false, range: [1000, 1000] } },
    // { channels: { mutable: false, range: [2, 2] } }
]


const items:MyItems[] = [
    { 
        label: "AllPass", 
        value: "allpass", 
        opts: defaultOpts
    },
    { 
        label: "BandPass",
        value: "bandpass",
        opts: defaultOpts
    },
    { 
        label: "ButterWorth", 
        value: "butterworth", 
        opts: defaultOpts
    },
    { 
        label: "HighPass",
        value: "highpass",
        opts: defaultOpts
    },
    { 
        label: "HighShelf",
        value: "highshelf",
        opts: defaultOpts 
    },
    { 
        label: "LowPass",
        value: "lowpass",
        opts: defaultOpts 
    },
    { 
        label: "LowShelf",
        value: "lowshelf",
        opts: defaultOpts 
    },
    { 
        label: "Notch",
        value: "notch",
        opts: defaultOpts 
    },
]


const effects:MyItems[] = [
    { label: "aaa", value: "aaa", opts: defaultOpts },
    { label: "bbb", value: "bbb", opts: defaultOpts },
    { label: "ccc", value: "ccc", opts: defaultOpts },
    { label: "ddd", value: "ddd", opts: defaultOpts },
    { label: "eee", value: "eee", opts: defaultOpts },
    { label: "fff", value: "fff", opts: defaultOpts },
    { label: "ggg", value: "ggg", opts: defaultOpts },
    { label: "hhh", value: "hhh", opts: defaultOpts }
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


const buildInitialState = (ifEnd:boolean, initial:number = 0) => {
    const state: Record<string, any> = {};

    items.forEach(item => {
        item.opts.forEach(opt => {
            state[`${item.value}.${Object.keys(opt)[0]}${ifEnd && "_end"}`] = [initial]
        });
    });
    return state;
};


function calcLogarithmicScale(x:number, lims:number[]){
    // changes position on slider to log10 value of number, which bound are in lims
    if (x == 0){
        return 0;
    }
    console.log(x, lims[0], lims[1]);
    let res = Math.log10( lims[0] ) + x/100 * ( Math.log10(lims[1]) - Math.log10(lims[0]) )
    console.log("RES:", res)
    console.log("x:", x, "RES POW: ", Math.pow(10, res))
    return res;
}


type linearAtrr = {
    percent: number;
    closestPowers: number[];
}


function calcLinearScale(x:number, lims:number[]):number {
    let lower_bound = Math.pow(10, Math.floor(Math.log10(x)))
    let upper_bound = Math.pow(10, Math.ceil(Math.log10(x)))

    if (lower_bound === upper_bound){
        if (lower_bound == lims[1]){
            return 100;
        }
        return 0;
    }
    console.log("LINEAR SCALE:", x, lower_bound, upper_bound);
    return x/(upper_bound - lower_bound) * 100;
}


function getBounds(x:number, lims: number[]):number[] {
    let lower_bound = Math.pow(10, Math.floor(Math.log10(x)))
    let upper_bound = Math.pow(10, Math.ceil(Math.log10(x)))
    console.log("BOUNDS", x, lower_bound, upper_bound)
    if (lower_bound === upper_bound){
        if (lower_bound == lims[1]){
            [lims[1]/10, lims[1]]
        }
        return [lower_bound, lower_bound*10]
    }
    return [lower_bound, upper_bound]
}


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

    const [Values, setValues] = useState<Record<string, any>>(buildInitialState(false));
    const [EndValues, setEndValues] = useState<Record<string, any>>(buildInitialState(true));

    const setSliderValue = (itemValue: string, opt: string, newValue: number) => {
        setValues(prev => ({
            ...prev,
            [`${itemValue}.${opt}`]: newValue
        }));
    };

    const setEndSliderValue = (itemValue: string, opt: string, newValue: number) => {
        setEndValues(prev => ({
            ...prev,
            [`${itemValue}.${opt}_end`]: newValue
        }));
    };

    useEffect(() => {
        console.log("EndValues changed:", EndValues);
    } , [EndValues]);



    const [status, setStatus] = useState<string>("logarithmic");

    const sliders = filteredItems.flatMap(obj => {
        return (
            <Fragment key={obj.value}>
                <Box p={5} bg="grey" rounded="2xl" maxW="100%" shadow="md">
                    <Text mb={2} fontWeight="medium" textAlign="center">{obj.value}</Text>
                    {
                        obj.opts.map(opt => {
                            const opt_key = Object.keys(opt)[0];
                            const state_key = `${obj.value}.${opt_key}`;
                            const Value = Values[state_key];
                            return (
                                <Fragment key={`${obj.value}.${opt_key}`}>
                                    <Slider.Root
                                        key={state_key}
                                        value={[Value]}

                                        onValueChange={(details) => {
                                            setSliderValue(obj.value, opt_key, details.value[0])
                                            console.log("SLIDER VALUE: ", Values[state_key])
                                        }}

                                        onValueChangeEnd={(details) => {
                                            setEndSliderValue(obj.value, opt_key, details.value[0]);
                                            console.log("SLIDER END VALUE: ", EndValues[`${state_key}_end`])
                                            // integration with backend will be here
                                        }} >

                                        <Slider.Label color="white"> {`${opt_key}`} </Slider.Label>
                                        <Slider.Control>
                                            <Slider.Track>
                                                <Slider.Range />
                                            </Slider.Track>
                                            <Slider.Thumbs />
                                        </Slider.Control>
                                    </Slider.Root>

                                    <Flex justify="space-between" align="center" mb={2} w="100%">
                                
                                        <Text> 
                                            {opt[opt_key].range[0]}
                                        </Text>

                                        <Text> Val:
                                            {
                                                ("logScale" in opt[opt_key] && opt[opt_key].logScale && status === "logarithmic") ?
                                                (
                                                    Math.round(Math.pow(10,
                                                     calcLogarithmicScale(EndValues[`${state_key}_end`], opt[opt_key].range)))
                                                     // round is for now, because it currenly only concerns Cuttof filter parameter
                                                ):
                                                (<b>{Math.round(EndValues[`${state_key}_end`])}</b>)
                                            }   
                                        </Text>

                                        {
                                           "logScale" in opt[opt_key] && opt[opt_key].logScale &&
                                           <Button bg={status === "linear" ? "green.400" : "red.400"}
                                                onClick={() => {
                                                    setStatus(prev => {
                                                        const newStatus = prev === "linear" ? "logarithmic" : "linear";

                                                        let actRange = getBounds(
                                                            EndValues[`${state_key}_end`],
                                                            opt[opt_key].range
                                                        );

                                                        let logVal = calcLogarithmicScale(
                                                            actRange[0] + (EndValues[`${state_key}_end`] / 100) * (actRange[1] - actRange[0]),
                                                            opt[opt_key].range );

                                                        let linVal = calcLinearScale(
                                                            Math.pow( 10, calcLogarithmicScale( EndValues[`${state_key}_end`], opt[opt_key].range )),
                                                            opt[opt_key].range );
                                                        
                                                        console.log("logval", logVal, "linval", linVal);
                                                        
                                                        setSliderValue(
                                                            obj.value,
                                                            opt_key,
                                                            newStatus === "logarithmic" ? logVal : linVal
                                                        );

                                                        setEndSliderValue(
                                                            obj.value,
                                                            opt_key,
                                                            newStatus === "logarithmic" ? logVal : linVal
                                                        );

                                                        return newStatus;
                                                        } 
                                                    )
                                                }
                                            }>
                                            {status}
                                            </Button>
                                        }
                                        <Text> {opt[opt_key].range[1]}</Text>
                                    </Flex>

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

                <Collapsible.Content maxW="100%" minW="100%">

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