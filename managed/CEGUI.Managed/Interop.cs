using System;
using System.Runtime.InteropServices;
using System.Text;

namespace CEGUI;

public sealed class CeguiException : Exception
{
    internal CeguiException(NativeResult result, string message)
        : base(message)
    {
        ResultCode = (int)result;
    }

    public int ResultCode { get; }
}

internal static class Interop
{
    internal static void Check(NativeResult result)
    {
        if (result == NativeResult.Success)
            return;

        string message = PtrToUtf8(NativeMethods.CEGUI_C_GetLastError());
        if (string.IsNullOrWhiteSpace(message))
            message = $"CEGUI native call failed with result {result}.";

        throw new CeguiException(result, message);
    }

    internal static string PtrToUtf8(IntPtr pointer)
    {
        if (pointer == IntPtr.Zero)
            return string.Empty;

        int length = 0;
        while (Marshal.ReadByte(pointer, length) != 0)
            checked { length++; }

        if (length == 0)
            return string.Empty;

        byte[] bytes = new byte[length];
        Marshal.Copy(pointer, bytes, 0, length);
        return Encoding.UTF8.GetString(bytes);
    }

    internal static UIntPtr ToUIntPtr(long value, string parameterName)
    {
        if (value < 0)
            throw new ArgumentOutOfRangeException(parameterName);

        return UIntPtr.Size == 8
            ? new UIntPtr(checked((ulong)value))
            : new UIntPtr(checked((uint)value));
    }

    internal static long FromUIntPtr(UIntPtr value)
    {
        ulong raw = value.ToUInt64();
        if (raw > long.MaxValue)
            throw new OverflowException("The native size_t value is too large for Int64.");
        return (long)raw;
    }
}
