export interface File {
  GetPath(): ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string;
  GetContent(): any;
  delete(): void;
}

export interface FileList {
  AddFile(_0: ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string, _1: any): void;
  delete(): void;
}

export interface Result {
  IsSuccess(): boolean;
  FileCount(): number;
  GetFile(_0: number): File;
  GetErrorCode(): ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string;
  delete(): void;
}

export interface MainModule {
  File: {new(): File};
  FileList: {new(): FileList};
  Result: {new(): Result};
  ConvertFileList(_0: FileList, _1: ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string): Result;
  ConvertFile(_0: ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string, _1: ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string, _2: any, _3: any, _4: any): Result;
}
