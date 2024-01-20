#include "packet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rbc/core/builtins.h>

rbc_dcp2_packet rbc_dcp2_packet_new(u8 size) {
	rbc_dcp2_packet self = {0};
	self.len = size;
	if (RBC_LIKELY(size <= sizeof(u8*))) {
		memset(self.data, 0, sizeof(self.data)); // NOLINT
	} else {
		self.ptr = calloc(size, 1);
	}
	return self;
}

void rbc_dcp2_packet_delete(rbc_dcp2_packet* self) {
	if (RBC_UNLIKELY(self->len > sizeof(u8*))) {
		free(self->ptr);
	}
}

u8 const* rbc_dcp2_packet_get_data(rbc_dcp2_packet const* self) {
	return RBC_LIKELY(self->len <= sizeof(u8*)) ? self->data : self->ptr;
}

u8* rbc_dcp2_packet_get_data_mut(rbc_dcp2_packet* self) {
	return (u8*) rbc_dcp2_packet_get_data(self);
}

static u8 header_crc(u8 const* data) {
	return 0x89 + data[0] + data[1] + data[2]; // NOLINT
}

static u8 packet_crc(u8 const* data, usize size) {
	u8 crc = 0;
	for (usize idx = 0; idx < size; ++idx) {
		crc += data[idx];
	}
	return crc;
}

/*
 * Packet: header data packet_crc?
 * Header: (dst:src) (cmd:ack:req) len header_crc (4 bytes)
 * If len == 0, packet_crc is absent.
 */
rbc_dcp2_result rbc_dcp2_packet_from_bytes(u8 const* data, usize size) {
	rbc_dcp2_result result = {.end = data, .error = RBC_DCP2_ERROR_NEED_MORE_DATA};
	if (size < 4) {
		return result;
	}

	if (data[3] != header_crc(data)) {
		result.error = RBC_DCP2_ERROR_INVALID_HEADER_CRC;
		return result;
	}

	u8 const len = data[2];
	u8 const packet_size = 4 + len + (len != 0);
	if (size < packet_size) {
		return result;
	}

	if (len != 0 && packet_crc(data, packet_size - 1) != data[packet_size - 1]) {
		result.error = RBC_DCP2_ERROR_INVALID_PACKET_CRC;
		return result;
	}

	rbc_dcp2_packet packet = rbc_dcp2_packet_new(len);
	packet.dst = data[0] & 0x0F;
	packet.src = (data[0] >> 4) & 0x0F;
	packet.cmd = data[1] & 0x1F;
	packet.ack = (data[1] >> 5) & 0x03;
	packet.req = (data[1] >> 7) & 0x01;
	packet.len = len;
	memcpy(rbc_dcp2_packet_get_data_mut(&packet), data + 4, len);
	return (rbc_dcp2_result){packet, data + packet_size, RBC_DCP2_ERROR_OK};
}

rbc_array_u8 rbc_dcp2_packet_to_bytes(rbc_dcp2_packet* self) {
	bool const need_packet_crc = self->len > 0;
	usize size = 4 + self->len + need_packet_crc;
	rbc_array_u8 bytes = rbc_array_u8_new(size);
	u8* data = bytes.data;
	data[0] = self->dst + (self->src << 4);
	data[1] = self->cmd + (self->ack << 5) + (self->req << 7); // NOLINT
	data[2] = self->len;
	data[3] = header_crc(data);
	memcpy(data + 4, rbc_dcp2_packet_get_data(self), self->len);
	if (need_packet_crc) {
		data[size - 1] = packet_crc(data, size - 1);
	}
	return bytes;
}

void rbc_dcp2_packet_print(rbc_dcp2_packet const* self) {
	printf("dcp2::packet{src: %u, dst: %u, cmd: %u, ack: %u, req: %u, data: [",
	    self->src, self->dst, self->cmd, self->ack, self->req);
	u8 const* data = rbc_dcp2_packet_get_data(self);
	if (self->len) {
		printf("0x%02X", data[0]);
	}
	for (u8 idx = 1; idx < self->len; ++idx) {
		printf(", 0x%02X", data[idx]);
	}
	printf("]}");
}
