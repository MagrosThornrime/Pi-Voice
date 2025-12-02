"use client";
import { Box, Checkbox, Button, Fieldset, Stack, CheckboxGroup, Heading, Flex, Grid, Slider} from "@chakra-ui/react";
import React, { useContext, ReactNode, createContext, useState} from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { useController, ControllerRenderProps, SubmitHandler, useForm, FieldError } from "react-hook-form"
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
    filters: ["AllPass", "BandPass"],
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


const items = [
  { label: "AllPass", value: "allpass", opts: ["a", "aa"] },
  { label: "BandPass", value: "bandpass" , opts: ["b", "bb"]},
  { label: "ButterWorth", value: "butterworth", opts: ["c", "cc"] },
  { label: "HighPass", value: "highpass", opts: ["d", "dd"] },
  { label: "HighShelf", value: "highshelf" , opts: ["e", "ee"] },
  { label: "LowPass", value: "lowpass", opts: ["f", "ff"]  },
  { label: "LowShelf", value: "lowshelf", opts: ["g", "gg"] },
  { label: "Notch", value: "notch", opts: ["h", "hh"] },
]


const effects = [
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
  ifButton:boolean;
  headerText: string;
  buttonText?: string;
  error?: FieldError;
  invalid?: boolean;
};


const buildInitialState = () => {
  const state: Record<string, any> = {};

  items.forEach(item => {
    item.opts.forEach(opt => {
      state[`${item.value}.${opt}`] = [10]
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
){
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
                    onValueChange={field.onChange}
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


function SlidersItems() {

    const { data } = useFilters();

    const filteredItems = items.filter(item =>
        data.filters.includes(item.label)
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

    return (
        <Box>
            {
                filteredItems.map(obj => obj.opts.map(
                        opt => {
                            const key = `${obj.value}.${opt}`;
                            const Value = Values[key];
                            console.log(key, Value)
                            return (
                                <Slider.Root key={key}
                                    value={[Value]}
                                    onValueChange={(details) => setSliderValue(obj.value, opt, details.value[0])}
                                    onValueChangeEnd={(details) => {
                                        console.log("ustawiam dla ", obj.label);
                                        setEndSliderValue(obj.value, `${opt}_end`, details.value[0]);
                                        // ctrl.onEnd(e.value[0]);
                                    }} >

                                    <Slider.Control>
                                        <Slider.Track>
                                            <Slider.Range />
                                        </Slider.Track>
                                        <Slider.Thumbs />
                                    </Slider.Control>
                                </Slider.Root>
                            )

                        }
                    )
                )
            }
        </Box>
    )

}


function Page(){

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


    return(
        <Box minH="100vh" bg="gray.50" p={10}>
            <form onSubmit={
                handleSubmit((data) => {
                    setData(data);
                    console.log(data.filters, data.effects)
                }
            )
            } >
                {/* <Stack direction="row" gap={60} justifyContent="center" alignItems="center" > */}
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
                    ifButton={true}
                    headerText="Select effects"
                    buttonText="HAHAHA">

                </CheckboxesWithHeading>
                {/* </Stack> */}
            </form>

            <SlidersItems/>

        </Box>
    )
}


export default function Home() {
    return (
        <FiltersProvider>
           <Page/>
        </FiltersProvider>
    )
}