"use client";
import { Box, Checkbox, Button, Fieldset, CheckboxGroup, Flex, Grid} from "@chakra-ui/react";
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
        <form onSubmit={handleSubmit((data) => console.log(data))}>
            <Fieldset.Root invalid={invalid}>
                <Fieldset.Legend color={"white"}>Select filters</Fieldset.Legend>
                <CheckboxGroup
                    invalid={invalid}
                    value={framework.field.value}
                    onValueChange={framework.field.onChange}
                    name={framework.field.name}
                >
                    <Fieldset.Content>
                        {items.map((item) => (
                            <Checkbox.Root key={item.value} value={item.value}>
                                <Checkbox.HiddenInput />
                                <Checkbox.Control />
                                <Checkbox.Label>{item.label}</Checkbox.Label>
                            </Checkbox.Root>
                        ))}
                    </Fieldset.Content>
                </CheckboxGroup>
                {errors.framework && (
                    <Fieldset.ErrorText>{errors.framework.message}</Fieldset.ErrorText>
                )}

                <Button size="sm" type="submit" alignSelf="flex-start">
                    Submit
                </Button>

            </Fieldset.Root>
        </form>
    )
}