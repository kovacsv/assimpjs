import type { MainModule } from "./assimpjs.d.ts";

declare module "assimpjs" {
    export default function(): Promise<MainModule>;
}