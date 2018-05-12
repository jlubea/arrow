#! /usr/bin/env node

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

const fs = require('fs');
const path = require('path');
const streamToIterator = require('stream-to-iterator');

const encoding = 'binary';
const { util: { PipeIterator } } = require('../');
const { Table, serializeStream } = require('../');

(async () => {
    // Todo (ptaylor): implement `serializeStreamAsync` that accepts an
    // AsyncIterable<Buffer>, rather than aggregating into a Table first
    const in_ = streamToIterator(process.argv.length < 3 ? process.stdin :
        fs.createReadStream(path.resolve(process.argv[2]), { encoding }));
    const out = process.argv.length < 4 ? process.stdout :
        fs.createWriteStream(path.resolve(process.argv[3]), { encoding });
    new PipeIterator(serializeStream(await Table.fromAsync(readArrowFile(in_))), encoding).pipe(out);

})().catch((e) => { console.error(e); process.exit(1); });

async function readArrowFile(stream) {
    let buffer = Buffer.from([]);
    for await (let chunk of stream) {
        chunk = Buffer.isBuffer(chunk) ? chunk : Buffer.from(chunk, encoding);
        buffer = Buffer.concat([buffer, chunk], buffer.length + chunk.length);
    }
    return buffer;
}
