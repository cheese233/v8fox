import resolve from "@rollup/plugin-node-resolve";
import commonjs from "@rollup/plugin-commonjs";
import { defineConfig } from "rollup";
import { chromeExtension } from "rollup-plugin-chrome-extension";
import typescript from "@rollup/plugin-typescript";
export default defineConfig({
    input: "src/manifest.json",
    output: {
        dir: "dist",
        format: "esm",
        compact: true,
    },
    plugins: [
        // always put chromeExtension() before other plugins
        chromeExtension(),
        typescript(),
        resolve(),
        commonjs(),
    ],
});
