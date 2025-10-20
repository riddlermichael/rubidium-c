#pragma once

#include <rbc/containers/array.h>
#include <rbc/core/attributes.h>
#include <rbc/core/export.h>
#include <rbc/core/types.h>

enum rbc_dcp2_address {
	RBC_DCP2_ADDRESS_PC = 0x00,
	RBC_DCP2_ADDRESS_SENSOR = 0x01,
	RBC_DCP2_ADDRESS_MODEM = 0x02,
	RBC_DCP2_ADDRESS_ALL = 0x0F
};

typedef enum rbc_dcp2_address rbc_dcp2_address;

enum rbc_dcp2_ack {
	RBC_DCP2_ACK_OK,
	RBC_DCP2_ACK_ERROR,
	RBC_DCP2_ACK_BUSY,
	RBC_DCP2_ACK_INVALID
};

typedef enum rbc_dcp2_ack rbc_dcp2_ack;

struct rbc_dcp2_packet {
	u8 dst : 4; ///< destination address
	u8 src : 4; ///< source address
	u8 cmd : 5; ///< command
	u8 ack : 2; ///< acknowledgement
	u8 req : 1; ///< 0 if is request, 1 if is response
	u8 len; ///< length

	union {
		u8 data[sizeof(u8*)];
		u8* ptr; // used only if len > sizeof(u8*)
	};
};

typedef struct rbc_dcp2_packet rbc_dcp2_packet;

enum rbc_dcp2_error {
	RBC_DCP2_ERROR_OK,
	RBC_DCP2_ERROR_INVALID_HEADER_CRC,
	RBC_DCP2_ERROR_INVALID_PACKET_CRC,
	RBC_DCP2_ERROR_NEED_MORE_DATA,
};

typedef enum rbc_dcp2_error rbc_dcp2_error;

struct rbc_dcp2_result {
	rbc_dcp2_packet packet;
	u8 const* end;
	rbc_dcp2_error error;
};

typedef struct rbc_dcp2_result rbc_dcp2_result;

RBC_EXPORT rbc_dcp2_packet rbc_dcp2_packet_new(u8 size);
RBC_EXPORT void rbc_dcp2_packet_delete(rbc_dcp2_packet* self) RBC_NONNULL;
RBC_EXPORT u8 const* rbc_dcp2_packet_get_data(rbc_dcp2_packet const* self) RBC_NONNULL;
RBC_EXPORT u8* rbc_dcp2_packet_get_data_mut(rbc_dcp2_packet* self) RBC_NONNULL;
RBC_EXPORT rbc_dcp2_result rbc_dcp2_packet_from_bytes(u8 const* data, usize size);
RBC_EXPORT rbc_array_u8 rbc_dcp2_packet_to_bytes(rbc_dcp2_packet* self) RBC_NONNULL;

// FIXME remove it
void rbc_dcp2_packet_print(rbc_dcp2_packet const* self);
