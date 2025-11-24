"use client";
import { Box, Checkbox, Button, Fieldset, CheckboxGroup, Heading, Flex, Grid} from "@chakra-ui/react";
import React, { useState, useMemo} from "react";
import { standardSchemaResolver } from "@hookform/resolvers/standard-schema"
import { useController, SubmitHandler, useForm } from "react-hook-form"
import { z } from "zod"


const formSchema = z.object({
  framework: z.array(z.string()).max(3, {
    message: "You cannot select more than 3 effects.",
  }),
})

type FormData = z.infer<typeof formSchema>

const items = [
  { label: "AllPass", value: "allpass" },
  { label: "BandPass", value: "bandpass" },
  { label: "ButterWorth", value: "butterworth" },
  { label: "HighPass", value: "highpass" },
  { label: "HighShelf", value: "highshelf" },
  { label: "LowPass", value: "lowpass" },
  { label: "LowShelf", value: "lowshelf" },
  { label: "Notch", value: "notch" },
]

export default function Home() {
    const { handleSubmit, control, formState: { errors } } = useForm<FormData>({
        resolver: standardSchemaResolver(formSchema),
    })

    const framework = useController(
        {
            control,
            name: "framework",
            defaultValue: [],
        }
    )

    const invalid = !!errors.framework

    return (
        <Box minH="100vh" bg="gray.50" p={10}>
            <form onSubmit={handleSubmit((data) => console.log(data))}>

                <Heading size="3xl" textAlign="center" mb={10} color="teal.600">
                    Select Filters
                </Heading>

                <Fieldset.Root invalid={invalid}>
                    <Fieldset.Legend color={"teal.600"}> </Fieldset.Legend>
                    <CheckboxGroup color={"black"}
                        invalid={invalid}
                        value={framework.field.value}
                        onValueChange={framework.field.onChange}
                        name={framework.field.name}
                    >
                        <Fieldset.Content>
                            <Grid templateColumns={{
                                    base: "2fr",
                                    md: "repeat(4, 1fr)",
                                    lg: "repeat(4, 1fr)",
                                }}
                                gap={10} maxW="800px" mx="auto">

                                {
                                    items.map((item) => (
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
                        {errors.framework && (
                            <Fieldset.ErrorText>{errors.framework.message}</Fieldset.ErrorText>
                        )}
                        <Box h="2" />
                        <Button size="sm" type="submit" alignSelf="flex-start">
                            Submit
                        </Button>
                    </Box>
                    
                </Fieldset.Root>
            </form>
        </Box>
    )
}