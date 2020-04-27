#include "test_fft16.h"
void generated_double_fft16(
	/* int n */
	double * in,
	double * out) {
  out[0]=((((in[0]+in[16])+(in[8]+in[24]))+((in[4]+in[20])+(in[12]+in[28])))+(((in[2]+in[18])+(in[10]+in[26]))+((in[6]+in[22])+(in[14]+in[30]))));
  out[1]=((((in[1]+in[17])+(in[9]+in[25]))+((in[5]+in[21])+(in[13]+in[29])))+(((in[3]+in[19])+(in[11]+in[27]))+((in[7]+in[23])+(in[15]+in[31]))));
  out[2]=((((in[0]-in[16])+(in[9]-in[25]))+((((in[4]-in[20])+(in[13]-in[29]))*0.7071067811865476)-(((in[5]-in[21])-(in[12]-in[28]))*-0.7071067811865476)))+(((((in[2]-in[18])+(in[11]-in[27]))+((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*0.9238795325112867)-((((in[3]-in[19])-(in[10]-in[26]))+((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*-0.3826834323650898)));
  out[3]=((((in[1]-in[17])-(in[8]-in[24]))+((((in[4]-in[20])+(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])-(in[12]-in[28]))*0.7071067811865476)))+(((((in[2]-in[18])+(in[11]-in[27]))+((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)+((((in[3]-in[19])-(in[10]-in[26]))+((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*0.9238795325112867)));
  out[4]=((((in[0]+in[16])-(in[8]+in[24]))+((in[5]+in[21])-(in[13]+in[29])))+(((((in[2]+in[18])-(in[10]+in[26]))+((in[7]+in[23])-(in[15]+in[31])))*0.7071067811865476)-((((in[3]+in[19])-(in[11]+in[27]))-((in[6]+in[22])-(in[14]+in[30])))*-0.7071067811865476)));
  out[5]=((((in[1]+in[17])-(in[9]+in[25]))-((in[4]+in[20])-(in[12]+in[28])))+(((((in[2]+in[18])-(in[10]+in[26]))+((in[7]+in[23])-(in[15]+in[31])))*-0.7071067811865476)+((((in[3]+in[19])-(in[11]+in[27]))-((in[6]+in[22])-(in[14]+in[30])))*0.7071067811865476)));
  out[6]=((((in[0]-in[16])-(in[9]-in[25]))+((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)-(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))+(((((in[2]-in[18])-(in[11]-in[27]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*0.3826834323650898)-((((in[3]-in[19])+(in[10]-in[26]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)));
  out[7]=((((in[1]-in[17])+(in[8]-in[24]))+((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))+(((((in[2]-in[18])-(in[11]-in[27]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)+((((in[3]-in[19])+(in[10]-in[26]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*0.3826834323650898)));
  out[8]=((((in[0]+in[16])+(in[8]+in[24]))-((in[4]+in[20])+(in[12]+in[28])))+(((in[3]+in[19])+(in[11]+in[27]))-((in[7]+in[23])+(in[15]+in[31]))));
  out[9]=((((in[1]+in[17])+(in[9]+in[25]))-((in[5]+in[21])+(in[13]+in[29])))-(((in[2]+in[18])+(in[10]+in[26]))-((in[6]+in[22])+(in[14]+in[30]))));
  out[10]=((((in[0]-in[16])+(in[9]-in[25]))-((((in[4]-in[20])+(in[13]-in[29]))*0.7071067811865476)-(((in[5]-in[21])-(in[12]-in[28]))*-0.7071067811865476)))+(((((in[2]-in[18])+(in[11]-in[27]))-((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)-((((in[3]-in[19])-(in[10]-in[26]))-((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*-0.9238795325112867)));
  out[11]=((((in[1]-in[17])-(in[8]-in[24]))-((((in[4]-in[20])+(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])-(in[12]-in[28]))*0.7071067811865476)))+(((((in[2]-in[18])+(in[11]-in[27]))-((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)+((((in[3]-in[19])-(in[10]-in[26]))-((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*-0.3826834323650898)));
  out[12]=((((in[0]+in[16])-(in[8]+in[24]))-((in[5]+in[21])-(in[13]+in[29])))+(((((in[2]+in[18])-(in[10]+in[26]))-((in[7]+in[23])-(in[15]+in[31])))*-0.7071067811865476)-((((in[3]+in[19])-(in[11]+in[27]))+((in[6]+in[22])-(in[14]+in[30])))*-0.7071067811865476)));
  out[13]=((((in[1]+in[17])-(in[9]+in[25]))+((in[4]+in[20])-(in[12]+in[28])))+(((((in[2]+in[18])-(in[10]+in[26]))-((in[7]+in[23])-(in[15]+in[31])))*-0.7071067811865476)+((((in[3]+in[19])-(in[11]+in[27]))+((in[6]+in[22])-(in[14]+in[30])))*-0.7071067811865476)));
  out[14]=((((in[0]-in[16])-(in[9]-in[25]))-((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)-(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))+(((((in[2]-in[18])-(in[11]-in[27]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)-((((in[3]-in[19])+(in[10]-in[26]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)));
  out[15]=((((in[1]-in[17])+(in[8]-in[24]))-((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))+(((((in[2]-in[18])-(in[11]-in[27]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)+((((in[3]-in[19])+(in[10]-in[26]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)));
  out[16]=((((in[0]+in[16])+(in[8]+in[24]))+((in[4]+in[20])+(in[12]+in[28])))-(((in[2]+in[18])+(in[10]+in[26]))+((in[6]+in[22])+(in[14]+in[30]))));
  out[17]=((((in[1]+in[17])+(in[9]+in[25]))+((in[5]+in[21])+(in[13]+in[29])))-(((in[3]+in[19])+(in[11]+in[27]))+((in[7]+in[23])+(in[15]+in[31]))));
  out[18]=((((in[0]-in[16])+(in[9]-in[25]))+((((in[4]-in[20])+(in[13]-in[29]))*0.7071067811865476)-(((in[5]-in[21])-(in[12]-in[28]))*-0.7071067811865476)))-(((((in[2]-in[18])+(in[11]-in[27]))+((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*0.9238795325112867)-((((in[3]-in[19])-(in[10]-in[26]))+((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*-0.3826834323650898)));
  out[19]=((((in[1]-in[17])-(in[8]-in[24]))+((((in[4]-in[20])+(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])-(in[12]-in[28]))*0.7071067811865476)))-(((((in[2]-in[18])+(in[11]-in[27]))+((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)+((((in[3]-in[19])-(in[10]-in[26]))+((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*0.9238795325112867)));
  out[20]=((((in[0]+in[16])-(in[8]+in[24]))+((in[5]+in[21])-(in[13]+in[29])))-(((((in[2]+in[18])-(in[10]+in[26]))+((in[7]+in[23])-(in[15]+in[31])))*0.7071067811865476)-((((in[3]+in[19])-(in[11]+in[27]))-((in[6]+in[22])-(in[14]+in[30])))*-0.7071067811865476)));
  out[21]=((((in[1]+in[17])-(in[9]+in[25]))-((in[4]+in[20])-(in[12]+in[28])))-(((((in[2]+in[18])-(in[10]+in[26]))+((in[7]+in[23])-(in[15]+in[31])))*-0.7071067811865476)+((((in[3]+in[19])-(in[11]+in[27]))-((in[6]+in[22])-(in[14]+in[30])))*0.7071067811865476)));
  out[22]=((((in[0]-in[16])-(in[9]-in[25]))+((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)-(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))-(((((in[2]-in[18])-(in[11]-in[27]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*0.3826834323650898)-((((in[3]-in[19])+(in[10]-in[26]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)));
  out[23]=((((in[1]-in[17])+(in[8]-in[24]))+((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))-(((((in[2]-in[18])-(in[11]-in[27]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)+((((in[3]-in[19])+(in[10]-in[26]))+((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*0.3826834323650898)));
  out[24]=((((in[0]+in[16])+(in[8]+in[24]))-((in[4]+in[20])+(in[12]+in[28])))-(((in[3]+in[19])+(in[11]+in[27]))-((in[7]+in[23])+(in[15]+in[31]))));
  out[25]=((((in[1]+in[17])+(in[9]+in[25]))-((in[5]+in[21])+(in[13]+in[29])))+(((in[2]+in[18])+(in[10]+in[26]))-((in[6]+in[22])+(in[14]+in[30]))));
  out[26]=((((in[0]-in[16])+(in[9]-in[25]))-((((in[4]-in[20])+(in[13]-in[29]))*0.7071067811865476)-(((in[5]-in[21])-(in[12]-in[28]))*-0.7071067811865476)))-(((((in[2]-in[18])+(in[11]-in[27]))-((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)-((((in[3]-in[19])-(in[10]-in[26]))-((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*-0.9238795325112867)));
  out[27]=((((in[1]-in[17])-(in[8]-in[24]))-((((in[4]-in[20])+(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])-(in[12]-in[28]))*0.7071067811865476)))-(((((in[2]-in[18])+(in[11]-in[27]))-((((in[6]-in[22])+(in[15]-in[31]))*0.7071067811865476)-(((in[7]-in[23])-(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)+((((in[3]-in[19])-(in[10]-in[26]))-((((in[6]-in[22])+(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])-(in[14]-in[30]))*0.7071067811865476)))*-0.3826834323650898)));
  out[28]=((((in[0]+in[16])-(in[8]+in[24]))-((in[5]+in[21])-(in[13]+in[29])))-(((((in[2]+in[18])-(in[10]+in[26]))-((in[7]+in[23])-(in[15]+in[31])))*-0.7071067811865476)-((((in[3]+in[19])-(in[11]+in[27]))+((in[6]+in[22])-(in[14]+in[30])))*-0.7071067811865476)));
  out[29]=((((in[1]+in[17])-(in[9]+in[25]))+((in[4]+in[20])-(in[12]+in[28])))-(((((in[2]+in[18])-(in[10]+in[26]))-((in[7]+in[23])-(in[15]+in[31])))*-0.7071067811865476)+((((in[3]+in[19])-(in[11]+in[27]))+((in[6]+in[22])-(in[14]+in[30])))*-0.7071067811865476)));
  out[30]=((((in[0]-in[16])-(in[9]-in[25]))-((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)-(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))-(((((in[2]-in[18])-(in[11]-in[27]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)-((((in[3]-in[19])+(in[10]-in[26]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)));
  out[31]=((((in[1]-in[17])+(in[8]-in[24]))-((((in[4]-in[20])-(in[13]-in[29]))*-0.7071067811865476)+(((in[5]-in[21])+(in[12]-in[28]))*-0.7071067811865476)))-(((((in[2]-in[18])-(in[11]-in[27]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)-(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.3826834323650898)+((((in[3]-in[19])+(in[10]-in[26]))-((((in[6]-in[22])-(in[15]-in[31]))*-0.7071067811865476)+(((in[7]-in[23])+(in[14]-in[30]))*-0.7071067811865476)))*-0.9238795325112867)));
}
