/*
 * opcua_encodingLayer.c
 *
 *  Created on: Jan 14, 2014
 *      Author: opcua
 */
#include "opcua_encodingLayer.h"
#include "opcua_binaryEncDec.h"
#include "opcua_types.h"
#include "opcua_builtInDatatypes.h"

/**
* IntegerId
* Part: 4
* Chapter: 7.13
* Page: 118
*/
T_IntegerId convertToIntegerId(char* buf, int pos){
	return convertToUInt32(buf, pos);
}

/**
* DiagnosticInfo
* Part: 4
* Chapter: 7.9
* Page: 116
*/
T_DiagnosticInfo convertToDiagnosticInfo(char* buf, int pos){
	T_DiagnosticInfo tmpTDiagnosticInfo;
	int counter = 0;
	tmpTDiagnosticInfo.namespaceUri = convertToInt32(buf, counter);
	counter += sizeof(tmpTDiagnosticInfo.namespaceUri);
	tmpTDiagnosticInfo.symbolicId = convertToInt32(buf, counter);
	counter += sizeof(tmpTDiagnosticInfo.symbolicId);
	tmpTDiagnosticInfo.locale = convertToInt32(buf, counter);
	counter += sizeof(tmpTDiagnosticInfo.locale);
	tmpTDiagnosticInfo.localizesText = convertToInt32(buf, counter);
	counter += sizeof(tmpTDiagnosticInfo.localizesText);
	tmpTDiagnosticInfo.additionalInfo = convertToUAString(buf, counter);
	counter += sizeof(tmpTDiagnosticInfo.additionalInfo);
	tmpTDiagnosticInfo.innerStatusCode = convertToUAStatusCode(buf, pos);
	counter += sizeof(tmpTDiagnosticInfo.innerStatusCode);

	//If the Flag InnerDiagnosticInfo is set, then the DiagnosticInfo will be encoded
	if((tmpTDiagnosticInfo.innerStatusCode & DIEMT_INNER_DIAGNOSTIC_INFO) == 1){
		tmpTDiagnosticInfo.innerDiagnosticInfo = convertToTDiagnosticInfo(buf, counter);
	}

	return tmpTDiagnosticInfo;
}

/**
* RequestHeader
* Part: 4
* Chapter: 7.26
* Page: 132
*/
T_RequestHeader decodeRequestHeader(char* buf)
{
	T_RequestHeader tmpRequestHeader;
	int counter = 0;
	//ToDo: counter needs the length of the buffer,
	//		strings have in this type just the size of the pointer not of the content
	tmpRequestHeader.authenticationToken = convertToUANodeId(buf, counter);
	if (tmpRequestHeader.authenticationToken.EncodingByte == NIEVT_STRING)
	{
		counter =
				sizeof(tmpRequestHeader.authenticationToken.EncodingByte)
						+ sizeof(tmpRequestHeader.authenticationToken.Namespace)
						+ sizeof(tmpRequestHeader.authenticationToken.Identifier.String.Length)
						+ sizeof(tmpRequestHeader.authenticationToken.Identifier.String.Data);
	}
	else
	{
		counter = sizeof(tmpRequestHeader.authenticationToken);
	}
	tmpRequestHeader.timestamp = convertToUADateTime(buf, counter);
	counter += sizeof(tmpRequestHeader.timestamp);
	tmpRequestHeader.requestHandle = convertToTIntegerId(buf, counter);
	counter += sizeof(tmpRequestHeader.requestHandle);
	tmpRequestHeader.returnDiagnostics = convertToUInt32(buf, counter);
	counter += sizeof(tmpRequestHeader.returnDiagnostics);
	tmpRequestHeader.auditEntryId = convertToUAString(buf, counter);
	counter += sizeof(tmpRequestHeader.auditEntryId);
	tmpRequestHeader.timeoutHint = convertToUInt32(buf, counter);
	counter += sizeof(tmpRequestHeader.timeoutHint);
	// AdditionalHeader will stay empty, need to be changed if there is relevant information

	return tmpRequestHeader;
}

/**
* ResponseHeader
* Part: 4
* Chapter: 7.27
* Page: 133
*/
decodeResponseHeader(const T_ResponseHeader *responseHeader, AD_RawMessage *buf)
{

	int counter = 0;
	responseHeader->timestamp = convertToUADateTime(buf->message, counter);
	counter += sizeof(responseHeader->timestamp);
	responseHeader->requestHandle = convertToTIntegerId(buf->message, counter);
	counter += sizeof(responseHeader->requestHandle);
	responseHeader->serviceResult = convertToUAStatusCode(buf->message, counter);
	counter += sizeof(responseHeader->serviceResult);
	responseHeader->serviceDiagnostics = convertToTDiagnosticInfo(buf->message, counter);

}

void decodeMessage_test()
{

	char testMessage = {01,0x20,0xbe,0x01,0x20,0x20,0xf2,0xd6,0xd6,0xc9,0x01,0x00,0xbe,0x01,0x00,0x00,0xf2,0xd6,0xd6,0xc9,0x87,0x0b,0xcf,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0xe0,0x93,0x04,0x00,0x,0x};
	AD_RawMessage rawMessage;
	rawMessage.message = testMessage;
	rawMessage.length = 64;
	decodeMessage(testMessage);
}
/*
 * builds a message structure by decoding a byte stream
 */
UA_ExtensionObject processMessage(AD_RawMessage *rawMessage)
{
	UA_NodeId tmpNodeId = convertToUANodeId(rawMessage,0);


	//callServiceHandler(tmpNodeId,rawMessage);
}


