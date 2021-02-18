#include <OpenHome/TIpAddressUtils.h>
#include <OpenHome/Private/Ascii.h>

using namespace OpenHome;

TBool TIpAddressUtils::Equal(const TIpAddress& aAddr1, const TIpAddress& aAddr2)
{
    if ((aAddr1.family == aAddr2.family) &&
    ((aAddr1.family == kFamilyV6 && (aAddr1.v6 == aAddr2.v6)) ||
        (aAddr1.family == kFamilyV4 && (aAddr1.v4 == aAddr2.v4)))) {
        return true;
    }
    return false;
}

TBool TIpAddressUtils::LessThan(const TIpAddress& aAddr1, const TIpAddress& aAddr2)
{
    if (aAddr1.family != aAddr2.family) {
        return (aAddr1.family == kFamilyV4) ? true : false;
    }

    if (aAddr1.family == kFamilyV6) {
        for (TUint i = 0; i < 16; i++) {
            if (aAddr1.v6[i] < aAddr2.v6[i])
                return true;
        }
        return false;
    }
    else {
        return (aAddr1.v4 < aAddr2.v4);
    }
}

TIpAddress TIpAddressUtils::ApplyMask(const TIpAddress& aAddr1, const TIpAddress& aAddr2)
{
    ASSERT(aAddr1.family == aAddr2.family);
    TIpAddress address;
    if (aAddr1.family == kFamilyV6) {
        address.family = kFamilyV6;
        for (TUint i = 0; i < 16; i++) {
            address.v6[i] = (aAddr1.v6[i] & aAddr2.v6[i]);
        }
    }
    else {
        address.family = kFamilyV4;
        address.v4 = (aAddr1.v4 & aAddr2.v4);
    }
    return address;
}

TBool TIpAddressUtils::IsZero(const TIpAddress& aAddr)
{
    if (aAddr.family == kFamilyV6) {
        for (TUint i = 0; i < 16; i++) {
            if (aAddr.v6[i] != 0x00) {
                return false;
            }
        }
        return true;
    }
    else {
        return (aAddr.v4 == kTIpAddressEmpty.v4);
    }
}

void TIpAddressUtils::ToString(const TIpAddress& aAddr, Bwx& aAddressBuffer)
{
    (aAddr.family == kFamilyV6) ? AddressToStringV6(aAddressBuffer, aAddr) : AddressToStringV4(aAddressBuffer, aAddr);
}

void TIpAddressUtils::AddressToStringV4(Bwx& aAddressBuffer, const TIpAddress& aAddress)
{
    ASSERT(aAddressBuffer.MaxBytes() - aAddressBuffer.Bytes() >= kMaxAddressBytes);
#ifdef DEFINE_LITTLE_ENDIAN
    (void)Ascii::AppendDec(aAddressBuffer, aAddress.v4&0xff);
    aAddressBuffer.Append('.');
    (void)Ascii::AppendDec(aAddressBuffer, (aAddress.v4>>8)&0xff);
    aAddressBuffer.Append('.');
    (void)Ascii::AppendDec(aAddressBuffer, (aAddress.v4>>16)&0xff);
    aAddressBuffer.Append('.');
    (void)Ascii::AppendDec(aAddressBuffer, (aAddress.v4>>24)&0xff);
#elif defined DEFINE_BIG_ENDIAN
    (void)Ascii::AppendDec(aAddressBuffer, (aAddress.v4>>24)&0xff);
    aAddressBuffer.Append('.');
    (void)Ascii::AppendDec(aAddressBuffer, (aAddress.v4>>16)&0xff);
    aAddressBuffer.Append('.');
    (void)Ascii::AppendDec(aAddressBuffer, (aAddress.v4>>8)&0xff);
    aAddressBuffer.Append('.');
    (void)Ascii::AppendDec(aAddressBuffer, aAddress.v4&0xff);
#else
# error No endianess defined
#endif
    aAddressBuffer.PtrZ();
}

void TIpAddressUtils::AddressToStringV6(Bwx& aAddressBuffer, const TIpAddress& aAddress)
{
    ASSERT(aAddressBuffer.MaxBytes() - aAddressBuffer.Bytes() >= kMaxAddressBytes);

    // Add the 2 bytes in each field (8 fields in an IPv6 addr) to thier own local buffer, removing leading zeros as necessary
    Bws<4> fields[8];
    for (TUint i = 0; i < 8; i++) {
        TUint byte = i * 2;
        // Combine the two bytes into a singular group (e.g., FFFF:)
        TUint16 fieldAsNumber = (aAddress.v6[byte] << 8) + aAddress.v6[byte+1];
        // Trim leading zeros for the field and append to local buffer
        if (!Ascii::AppendHexTrim(fields[i], fieldAsNumber)) {
            // We haven't written any bytes for this field; substitute '0'
            (void)Ascii::AppendHex(fields[i], Brn("0"));
        }
    }

    // Identify consecutive fields of ':0000:', priority is given to left-most groups for the '::' nomenclature
    TInt zeroFieldIndex = -1;               // Location of the left-most zero field
    TInt zeroFieldIndexPrev = 0;           // Location of the left-most zero field if we encounter another unique zero field towards the RHS of the address
    TInt consecutiveZeroFields = 0;        // Number of consecutive zero fields starting from zeroFieldIndex
    TInt consecutiveZeroFieldsPrev = 0;    // Number of consecutive zero fields starting from zeroFieldIndexPrev if we encounter another unique zero field

    for (TInt j = 0; j < 8; j++) {
        if (fields[j] == Brn("0")) {
            // If this zero field immediately follows a zero field, increment the number of consecutive zero fields
            if (zeroFieldIndex  == (j - consecutiveZeroFields)) {
                consecutiveZeroFields++;
                continue;
            }
            // In this case we have encountered a zero field independent of the previous zero field, cache the previous group of fields
            else if (consecutiveZeroFieldsPrev < consecutiveZeroFields) {
                zeroFieldIndexPrev = zeroFieldIndex;
                consecutiveZeroFieldsPrev = consecutiveZeroFields;
            }

            // Default here for the cases where a) no zero field has been identified yet or b) we have cached the previous group of fields and will compare the sizes of each group in future
            zeroFieldIndex = j;
            consecutiveZeroFields = 1;
        }
    }

    // Determine if we have any zero fields or groups of zero fields. If we have groups which is greater. If they are equal left-most (any 'previous') group gets priority
    if (zeroFieldIndex != -1) {
        TBool curr = (consecutiveZeroFields > consecutiveZeroFieldsPrev);
        if (!curr) {
            consecutiveZeroFields = consecutiveZeroFieldsPrev;
            zeroFieldIndex = zeroFieldIndexPrev;
        }
    }

    // Now construct the string, iterate through each group
    for (TInt k = 0; k < 8; k++) {
        if (k == zeroFieldIndex) {
            aAddressBuffer.Append(':');
            k += (consecutiveZeroFields - 1); // remove one from consecutiveZeroFields so that we iterate onto the next field on continuation of loop
            continue;
        }

        aAddressBuffer.Append(fields[k]);
        // Do not apply colon de-limiter to the end of the string
        if (k < 7) {
            aAddressBuffer.Append(':');
        }
    }
    aAddressBuffer.PtrZ();
}