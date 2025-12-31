"use client";

import { ReactNode } from "react";
import { FiltersProvider, FiltersParamsProvider } from "@/app/utils/context_utils";

export function Providers({ children }: { children: ReactNode }) {
  return (
    <FiltersParamsProvider>
      <FiltersProvider>
        {children}
      </FiltersProvider>
    </FiltersParamsProvider>
  );
}