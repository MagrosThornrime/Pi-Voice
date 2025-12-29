import { FiltersData } from "../effects_filters/page";
import { useContext, ReactNode, createContext, useState } from "react";

type FiltersContextType = {
    data: FiltersData;
    setData: (value: FiltersData) => void;
};

export type orderedDataType = {
    filters: string[];
    effects: string[];
}

type FiltersOrderContextType = {
    orderedData: orderedDataType;
    setOrderedData: React.Dispatch<React.SetStateAction<orderedDataType>>;
};

const FiltersOrderedContext = createContext<FiltersOrderContextType | undefined>(undefined);

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

export function FiltersOrderedProvider({ children }: { children: ReactNode }) {
    const [orderedData, setOrderedData] = useState<orderedDataType>({
        filters: [],
        effects: []
    });

    return (
        <FiltersOrderedContext.Provider value={{ orderedData, setOrderedData }}>
            {children}
        </FiltersOrderedContext.Provider>
    );
}

export function useFilters() {
    const ctx = useContext(FiltersContext);
    if (!ctx) {
        throw new Error("useFilters must be used inside FiltersProvider");
    }
    return ctx;
}

export function useOrderedFilters() {
    const ctx = useContext(FiltersOrderedContext);
    if (!ctx) {
        throw new Error("useFilters must be used inside FiltersProvider");
    }
    return ctx;
}