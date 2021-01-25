#ifndef __UFSHCD_PLATFORM_H__
#define __UFSHCD_PLATFORM_H__

struct ufs_hba;
int ufshcd_clk_scaling_enable(struct ufs_hba *hba, int val);
