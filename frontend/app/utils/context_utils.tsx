"use client";
import { FiltersData } from "../effects_filters/page";
import { useContext, ReactNode, createContext, useState } from "react";
import { OptKey, FilterType, defaultOpts, defaultEffectOpts, EffectType, EffectOptionsMap, OptEffectKey,} from "./tables";
import { SliderProps } from "@/components/SlidersItems";
import { v4 as uuidv4 } from "uuid";


type FiltersContextType = {
    data: FiltersData;
    setData: React.Dispatch<React.SetStateAction<FiltersData>>;
};

export type setOpts = "Val" | "EndVal" | "Props";

export type OptParams = {
    Val: number;
    EndVal: number;
    Props: SliderProps;
}

export type FiltersParams = {
    group: "filters";
    record: Record<OptKey, OptParams>;
    value: FilterType;
}

export type EffectsParams<T extends EffectType = EffectType>= {
    group: "effects";
    record: Record<EffectOptionsMap[T], OptParams>;
    value: T;
}

export type ItemsParams = {
    params: EffectsParams | FiltersParams
    id: string;
}


export type FiltersParamsContextType = {
    paramsData: ItemsParams[];
    setParamsData: React.Dispatch<React.SetStateAction<ItemsParams[]>>;
};


const FiltersContext = createContext<FiltersContextType | undefined>(undefined);
const FiltersParamsContext = createContext<FiltersParamsContextType | undefined>(undefined);


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


export function FiltersParamsProvider({ children }: { children: ReactNode }) {
    const [paramsData, setParamsData] = useState<ItemsParams[]>([]);

    return (
        <FiltersParamsContext.Provider value={{ paramsData, setParamsData }}>
            {children}
        </FiltersParamsContext.Provider>
    );
}


export function useFilters() {
    const ctx = useContext(FiltersContext);
    if (!ctx) {
        throw new Error("useFilters must be used inside FiltersProvider");
    }
    return ctx;
}


export function useFiltersParams() {
    const ctx = useContext(FiltersParamsContext);
    if (!ctx) {
        throw new Error("useFilters must be used inside FiltersProvider");
    }
    return ctx;
}


export function useFiltersLogic() {
    const { paramsData, setParamsData } = useFiltersParams();

    const deleteFilterFromList = (idx: number) => {
        setParamsData(prev => {
            console.log("DELETING", idx, prev.length);

            const newParamsData = [...prev];
            newParamsData.splice(idx, 1);

            console.log("SIZE AFTER:", newParamsData.length);
            return newParamsData;
        });
    };

    const swapFiltersFromList = (idx1: number, idx2: number) => {
        setParamsData(prev => {
            console.log("SWAPPING", idx1, idx2, "SIZE BEFORE:", prev.length);

            const newParamsData = [...prev];

            const temp = newParamsData[idx1];
            newParamsData[idx1] = newParamsData[idx2];
            newParamsData[idx2] = temp;

            console.log("SIZE AFTER:", newParamsData.length);

            return newParamsData;
        });
    };

    const addFilterToList = (itemName: FilterType, names: OptKey[], idx: number, initial = 0) => {
        console.log("ADDING", itemName, idx);
        const newParams: ItemsParams = { id: uuidv4(),
            params: { group: "filters", value: itemName, record: {} as Record<OptKey, OptParams> }
        };
        const record = newParams.params.record as Record<OptKey, OptParams>;
        names.forEach((key, _) => {
            record[key] = { EndVal: initial, Val: initial, Props: { bounds: defaultOpts[key].range, actValue: initial } }
        })

        setParamsData(prev => {
            const newParamsData = [...prev];
            newParamsData.splice(idx, 0, newParams);
            return newParamsData;
        });

    }


    const addEffectToList = (itemName: EffectType, names: OptEffectKey[], idx: number, initial = 0) => {
        console.log("ADDING", itemName, idx);
        const newParams: ItemsParams = {
            id: uuidv4(),
            params: { group: "effects", value: itemName, record: {} as Record<OptEffectKey, OptParams> }
        };
        const record = newParams.params.record as Record<OptEffectKey, OptParams>;
        names.forEach((key, _) => {
            record[key] = { EndVal: initial, Val: initial, Props: { bounds: defaultEffectOpts[itemName][key].range, actValue: initial } }
        })

        setParamsData(prev => {
            const newParamsData = [...prev];
            newParamsData.splice(idx, 0, newParams);
            return newParamsData;
        });
    }

    const moveFilterInList = (idxFrom:number, idxTo:number) => {
        setParamsData(prev => {
            console.log("MOVING", idxFrom, idxTo);
            const newParamsData = [...prev];
            const elem = newParamsData.splice(idxFrom, 1)[0];
            newParamsData.splice(idxTo > idxFrom ? idxTo - 1 : idxTo, 0, elem); // after splice all elements are moved to the left
            return newParamsData;
        });
    }

    return { paramsData, swapFiltersFromList, addFilterToList, deleteFilterFromList, moveFilterInList, addEffectToList };
}