// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace Apache.Arrow.Flatbuf
{

using global::System;
using global::FlatBuffers;

/// ----------------------------------------------------------------------
/// A Buffer represents a single contiguous memory segment
public struct Buffer : IFlatbufferObject
{
  private Struct __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public void __init(int _i, ByteBuffer _bb) { __p.bb_pos = _i; __p.bb = _bb; }
  public Buffer __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  /// The relative offset into the shared memory page where the bytes for this
  /// buffer starts
  public long Offset { get { return __p.bb.GetLong(__p.bb_pos + 0); } }
  /// The absolute length (in bytes) of the memory buffer. The memory is found
  /// from offset (inclusive) to offset + length (non-inclusive).
  public long Length { get { return __p.bb.GetLong(__p.bb_pos + 8); } }

  public static Offset<Buffer> CreateBuffer(FlatBufferBuilder builder, long Offset, long Length) {
    builder.Prep(8, 16);
    builder.PutLong(Length);
    builder.PutLong(Offset);
    return new Offset<Buffer>(builder.Offset);
  }
};


}
