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
    index: number;
}


type MyItems = {
    label: string;
    value: string;
    opts: Record<string, Opt>;
};


type OptKey = "order" | "cutoff" | "gainDB" | "quality";


let defaultOpts: Record<OptKey, Opt> = {
    order: { mutable: true, continuous: false, logScale: false, range: [0, 1], step: 1, index: 3 } ,
    cutoff: { mutable: true, continuous: false, logScale: true, range: [10, 20000], index: 0 } ,
    gainDB: {mutable: true, continuous: true, logScale: false, range: [-24, 24], index: 2 },
    quality: {mutable: true, continuous: true, logScale: false, range: [0.1, 20.0], index: 1 }
    // sampleRate: { mutable: false, range: [1000, 1000] },
    // channels: { mutable: false, range: [2, 2] }
}


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
        label: "HighPass",
        value: "highpass",
        opts: defaultOpts
    },
    { 
        label: "LowPass",
        value: "lowpass",
        opts: defaultOpts 
    },
    {
        label: "HighShelf",
        value: "highshelf",
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
    {
        label: "PeakingEQ",
        value: "peakingeq",
        opts: defaultOpts
    },
]


const effects:MyItems[] = [
    { label: "echo", value: "echo", opts: defaultOpts },
    { label: "chorus", value: "chorus", opts: defaultOpts },
    { label: "delay", value: "delay", opts: defaultOpts },
    { label: "glitches", value: "glitches", opts: defaultOpts },
    { label: "reverb", value: "reverb", opts: defaultOpts },
    { label: "equalizer", value: "equalizer", opts: defaultOpts },
    { label: "fades", value: "fades", opts: defaultOpts },
    { label: "noise reduction", value: "noise reduction", opts: defaultOpts }
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


const buildInitialState = (ifEnd:boolean, initial:number|number[] = 0, ifBounds = false) => {
    const state: Record<string, any> = {};

    items.forEach(item => {
        (Object.entries(item.opts) as [OptKey, Opt][]).map(([key, opt]) => {
            if (!ifBounds){
                if (defaultOpts[key].mutable){
                    state[`${item.value}.${key}${ifEnd ? "_end" : ""}`] = [initial]
                }
                else{
                    state[`${item.value}.${key}${ifEnd ? "_end" : ""}`] = [50]
                }
            }
            else{
                if (defaultOpts[key].mutable){
                    state[`${item.value}.${key}`] = {bounds: defaultOpts[key].range, actValue: initial}
                }
                else{
                    state[`${item.value}.${key}`] = {bounds: defaultOpts[key].range, actValue: 50}
                }
            }
        });
    });
    return state;
};


function calcLogValueFromScale(x:number, lims:number[]){
    // changes position on slider to log10 value of number, which bound are in lims
    if (x == 0){
        return 0;
    }
    let res = Math.log10( lims[0] ) + x/100 * ( Math.log10(lims[1]) - Math.log10(lims[0]) )
    return res;
}


function calcLogaritmicPosFromLinear(x:number, linLims:number[], logLims:number[]){
    // changes position on slider for linear scale into logarithmic scale
    const actVal = linLims[0] + (linLims[1] - linLims[0]) * x/100
    const logMin = Math.log10(logLims[0]);
    const logMax = Math.log10(logLims[1]);
    const logVal = Math.log10(actVal);

    const logPos = (logVal - logMin) / (logMax - logMin);
    
    console.log("logPos:", logPos, "power:", Math.pow(10, calcLogValueFromScale(logPos, logLims)))
    return Math.round(logPos * 100);
}


function calcLinearPosFromLogarithmic(x:number, logLims:number[], linLims:number[]){
    const actVal = Math.pow(10, calcLogValueFromScale(x, logLims))
    return Math.round((actVal - linLims[0])/(linLims[1] - linLims[0]) * 100);
}


function getBounds(x:number, lims: number[]):number[] {
    let lower_bound = Math.pow(10, Math.floor(Math.log10(x)))
    let upper_bound = Math.pow(10, Math.ceil(Math.log10(x)))

    if (lower_bound === upper_bound){
        if (lower_bound == lims[1]){
            return [lower_bound/10, lower_bound]
        }
        return [lower_bound, lower_bound*10]
    }
    if (upper_bound > lims[1]){
        return [lower_bound, lims[1]]
    }

    return [lower_bound, upper_bound]
}


function calcValueFromLogScale(x:number, lims: number[]){
    return Math.pow(10, calcLogValueFromScale(x, lims));
}


function calcValueFromLinScale(x:number, lims:number[]){
    return lims[0] + x/100 * (lims[1] - lims[0]);
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


type SliderProps = {
    bounds: number[];
    actValue: number;
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

    const [Values, setValues] = useState<Record<string, any>>(buildInitialState(false, 0));
    const [EndValues, setEndValues] = useState<Record<string, any>>(buildInitialState(true, 0));
    const [Props, setProps] = useState<Record<string, SliderProps>>(buildInitialState(false, 0, true));

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

    const setSliderProps= (itemValue: string, opt: string, newProps: SliderProps) => {
        setProps(prev => ({
            ...prev,
            [`${itemValue}.${opt}`]: newProps
        }))
    }

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
                        ( Object.entries(obj.opts) as [OptKey, Opt][]).map(([key, opt]) => {
                            const opt_key = key;
                            const state_key = `${obj.value}.${opt_key}`;
                            const Value = Values[state_key];
                            console.log(Values);
                            console.log(obj);
                            return (
                                <Fragment key={`${obj.value}.${opt_key}`}>

                                    <Slider.Root
                                        key={state_key}
                                        value={[Value]}

                                        onValueChange={(details) => {
                                            const sliderVal = details.value[0]
                                            if (opt.mutable) {
                                                setSliderValue(obj.value, opt_key, sliderVal)

                                                setSliderProps(obj.value, opt_key, {
                                                    bounds: Props[state_key].bounds,
                                                    actValue: ("logScale" in opt && opt.logScale)
                                                        ? (status === "logarithmic"
                                                            ? calcValueFromLogScale(sliderVal, opt.range)
                                                            : calcValueFromLinScale(sliderVal, Props[state_key].bounds)
                                                        )
                                                        : calcValueFromLinScale(sliderVal, opt.range)
                                                })
                                            }
                                        }
                                        }


                                        onValueChangeEnd={async (details) => {
                                            const sliderVal = details.value[0]
                                            if (opt.mutable) {
                                                setEndSliderValue(obj.value, opt_key, sliderVal);

                                                setSliderProps(obj.value, opt_key,
                                                    {
                                                        bounds: Props[`${state_key}`].bounds,
                                                        actValue: ("logScale" in opt && opt.logScale) ?
                                                            (
                                                                status === "logarithmic" ?
                                                                    ( calcValueFromLogScale(sliderVal, opt.range) )
                                                                    :
                                                                    ( calcValueFromLinScale(sliderVal, Props[state_key].bounds) )
                                                            )
                                                            :
                                                            ( calcValueFromLinScale(sliderVal, opt.range) )

                                                    }
                                                )
                                            }

                                            console.log("SLIDER END VALUE: ", EndValues[`${state_key}_end`]);
                                            await setFilterParam(obj.value, defaultOpts[opt_key].index, Props[state_key].actValue);
                                        }}>

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
                                            {opt.mutable && Props[`${state_key}`].bounds[0]}
                                        </Text>

                                        <Text> Val:
                                            {
                                                opt.mutable ? Math.round(Props[`${state_key}`].actValue) : opt.range[0] 
                                            } 
                                        </Text> 

                                        {
                                           "logScale" in opt && opt.logScale &&
                                           <Button bg={status === "linear" ? "green.400" : "red.400"}
                                                onClick={() => {
                                                    setStatus(prev => {
                                                        const newStatus = prev === "linear" ? "logarithmic" : "linear";
                                                        const SliderVal = EndValues[`${state_key}_end`]

                                                        let logPos = calcLogaritmicPosFromLinear(SliderVal, Props[state_key].bounds, opt.range)

                                                        let actVal = newStatus === "logarithmic" ? 
                                                        ( calcValueFromLinScale(SliderVal, Props[state_key].bounds))
                                                        :
                                                        ( calcValueFromLogScale(SliderVal, opt.range) )

                                                        let linRange = getBounds(actVal, opt.range);
                                                        let linPos = calcLinearPosFromLogarithmic(SliderVal, opt.range, linRange);
                                                        
                                                        setSliderValue(
                                                            obj.value, opt_key,
                                                            newStatus === "logarithmic" ? logPos : linPos
                                                        );

                                                        setEndSliderValue(
                                                            obj.value, opt_key,
                                                            newStatus === "logarithmic" ? logPos : linPos
                                                        );

                                                        setSliderProps(
                                                            obj.value, opt_key,
                                                            newStatus === "logarithmic" ? 
                                                            ({ bounds: opt.range, actValue: actVal })
                                                            :
                                                            ({ bounds: linRange, actValue: actVal })
                                                        );

                                                        return newStatus;
                                                    })
                                                }
                                            }>
                                            { status }
                                            </Button>
                                        }

                                        <Text> {opt.mutable && Props[`${state_key}`].bounds[1]} </Text>
                                    </Flex>

                                    <Box h="5" />

                                </Fragment>
                            );
                        })
                    }
                </Box>
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

let currentFilters : string[] = [];

async function clearFilters(filtersNumber: number) {
    for(let i=filtersNumber-1; i>=0; i--){
        await window.synthAPI.pipelineRemove(i);
    }
}

async function addFilters(filters: string[]) {
    for (let [index, filter] of filters.entries()) {
        const filterNumber = items.findIndex(i => i.value === filter);
        await window.synthAPI.pipelineAddFilter(filterNumber, index);
    }
}

async function setFilterParam(filterName: string, param: number, value: number){
    const filterNumber = currentFilters.findIndex(i => i === filterName);
    await window.synthAPI.pipelineSetFilterParam(filterNumber, param, value)
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