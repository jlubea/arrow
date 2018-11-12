// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

import '../../Arrow';
import * as fs from 'fs';
import * as Path from 'path';
import { nodeToDOMStream } from './util';

import { Schema } from '../../../src/schema';
import { RecordBatch } from '../../../src/recordbatch';
import {
    ArrowDataSource,
    RecordBatchFileReader, AsyncRecordBatchFileReader,
}  from '../../../src/ipc/reader';

const simpleFilePath = Path.resolve(__dirname, `../../data/cpp/file/simple.arrow`);
const simpleFileData = fs.readFileSync(simpleFilePath);

function* iterableSource(buffer: Uint8Array) {
    let offset = 0, size = 0;
    while (offset < buffer.byteLength) {
        size = yield buffer.subarray(offset, offset +=
            (isNaN(+size) ? buffer.byteLength - offset : size));
    }
}

async function* asyncIterableSource(buffer: Uint8Array) {
    let offset = 0, size = 0;
    while (offset < buffer.byteLength) {
        size = yield buffer.subarray(offset, offset +=
            (isNaN(+size) ? buffer.byteLength - offset : size));
    }
}

describe('RecordBatchFileReader', () => {

    it('should read all messages from an Buffer of Arrow data in memory', () => {
        testSimpleRecordBatchFileReader(new ArrowDataSource(simpleFileData));
    });

    it('should read all messages from an Iterable that yields buffers of Arrow messages in memory', () => {
        testSimpleRecordBatchFileReader(new ArrowDataSource(iterableSource(simpleFileData)));
    });

    function testSimpleRecordBatchFileReader(source: ArrowDataSource) {

        let r: IteratorResult<RecordBatch>;

        const reader = source.open() as RecordBatchFileReader;

        expect(reader.readSchema()).toBeInstanceOf(Schema);

        r = reader.next();
        expect(r.done).toBe(false);
        expect(r.value).toBeInstanceOf(RecordBatch);

        r = reader.next();
        expect(r.done).toBe(false);
        expect(r.value).toBeInstanceOf(RecordBatch);

        r = reader.next();
        expect(r.done).toBe(false);
        expect(r.value).toBeInstanceOf(RecordBatch);

        expect(reader.next().done).toBe(true);
    }
});

describe('AsyncMessageReader', () => {

    it('should read all messages from a NodeJS ReadableFile', async () => {
        await testSimpleAsyncRecordBatchFileReader(new ArrowDataSource(fs.createReadStream(simpleFilePath)));
    });

    it('should read all messages from an AsyncIterable that yields buffers of Arrow messages in memory', async () => {
        await testSimpleAsyncRecordBatchFileReader(new ArrowDataSource(asyncIterableSource(simpleFileData)));
    });

    it('should read all messages from a whatwg ReadableStream', async () => {
        await testSimpleAsyncRecordBatchFileReader(new ArrowDataSource(
            nodeToDOMStream(fs.createReadStream(simpleFilePath))));
    });

    it('should read all messages from a whatwg ReadableByteStream', async () => {
        await testSimpleAsyncRecordBatchFileReader(new ArrowDataSource(
            nodeToDOMStream(fs.createReadStream(simpleFilePath), { type: 'bytes' })));
    });

    async function testSimpleAsyncRecordBatchFileReader(source: ArrowDataSource) {

        let r: IteratorResult<RecordBatch>;

        let reader = await source.open() as AsyncRecordBatchFileReader;

        expect(await reader.readSchema()).toBeInstanceOf(Schema);

        r = await reader.next();
        expect(r.done).toBe(false);
        expect(r.value).toBeInstanceOf(RecordBatch);

        r = await reader.next();
        expect(r.done).toBe(false);
        expect(r.value).toBeInstanceOf(RecordBatch);

        r = await reader.next();
        expect(r.done).toBe(false);
        expect(r.value).toBeInstanceOf(RecordBatch);

        expect((await reader.next()).done).toBe(true);
    }
});
