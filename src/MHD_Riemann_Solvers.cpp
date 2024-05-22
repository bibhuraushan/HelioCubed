#include "Proto.H"
#include "MHDOp.H"
#include "CommonTemplates.H"
#include "Proto_Timer.H"
// #include "Proto_WriteBoxData.H"
#include "MHD_Input_Parsing.H"
#include "MHD_Output_Writer.H"
#include "MHD_Constants.H"
#include "MHD_Mapping.H"
#include "MHD_Operator.H"
typedef BoxData<double,1,HOST> Scalar;
typedef BoxData<double,NUMCOMPS,HOST> Vector;

extern Parsefrominputs inputs;

/// @brief MHD_Riemann_Solvers namespace
namespace MHD_Riemann_Solvers {

	PROTO_KERNEL_START
	void roe8waveStateF(State& a_out,
	                    const State& a_W_lo,
	                    const State& a_W_hi,
	                    int a_dir,
	                    double a_gamma)
	{
		double gamma = a_gamma;

		double RhoL, UL, VL, WL, PGasL, BXL, BYL, BZL;
		double RhoR, UR, VR, WR, PGasR, BXR, BYR, BZR;



		if (a_dir == 0) {
			RhoL  = a_W_lo(0);
			RhoR  = a_W_hi(0);
			UL = a_W_lo(1);
			UR = a_W_hi(1);
			VL = a_W_lo(2);
			VR = a_W_hi(2);
			WL = a_W_lo(3);
			WR = a_W_hi(3);
			PGasL    = a_W_lo(4);
			PGasR    = a_W_hi(4);
			BXL   = a_W_lo(5);
			BXR   = a_W_hi(5);
			BYL   = a_W_lo(6);
			BYR   = a_W_hi(6);
			BZL   = a_W_lo(7);
			BZR   = a_W_hi(7);
		}
		if (a_dir == 1) {
			RhoL  = a_W_lo(0);
			RhoR  = a_W_hi(0);
			UL = a_W_lo(2);
			UR = a_W_hi(2);
			VL = a_W_lo(3);
			VR = a_W_hi(3);
			WL = a_W_lo(1);
			WR = a_W_hi(1);
			PGasL    = a_W_lo(4);
			PGasR    = a_W_hi(4);
			BXL   = a_W_lo(6);
			BXR   = a_W_hi(6);
			BYL   = a_W_lo(7);
			BYR   = a_W_hi(7);
			BZL   = a_W_lo(5);
			BZR   = a_W_hi(5);
		}
		if (a_dir == 2) {
			RhoL  = a_W_lo(0);
			RhoR  = a_W_hi(0);
			UL = a_W_lo(3);
			UR = a_W_hi(3);
			VL = a_W_lo(1);
			VR = a_W_hi(1);
			WL = a_W_lo(2);
			WR = a_W_hi(2);
			PGasL    = a_W_lo(4);
			PGasR    = a_W_hi(4);
			BXL   = a_W_lo(7);
			BXR   = a_W_hi(7);
			BYL   = a_W_lo(5);
			BYR   = a_W_hi(5);
			BZL   = a_W_lo(6);
			BZR   = a_W_hi(6);
		}

		if (PGasL < 0) PGasL = 0.0;
		if (PGasR < 0) PGasR = 0.0;


		double RUL, RVL, RWL, RUR, RVR, RWR;
		double PGas, Rho,  U,  V,  W, BX,  BY,  BZ;
		double BxBL, RUxUL, PL, EL, AL2, HL, coefRL;
		double BxBR, RUxUR, PR, ER, AR2, HR, coefRR;
		double BxBH, P, HAV, BXAV, DBX, DBY, DBZ;

		double aas, as, aaf, af, alf, als, c, cc, ccmi, ccma, dfs, X;
		double hyy, hyz, hzz, szb, h, kk, sih, z, del, dell2;
		double b, bb, BX2, BY2, BZ2, BYZ2, sqPiRho, sq2PiRho;
		double Aeigen1, Aeigen2, Aeigen3, Aeigen4, Aeigen5, Aeigen6;
		double Aeigen7, Aeigen8;
		double eigen1,  eigen2,  eigen3,  eigen4,  eigen5,  eigen6;
		double eigen7,  eigen8,  eigmax;
		double fl1, fl2, fl3, fl4, fl5, fl6, fl7, fl8;
		double fr1, fr2, fr3, fr4, fr5, fr6, fr7, fr8;
		double du1, du2, du3, du4, du5, du6, du7, du8;
		double lu1, lu2, lu3, lu4, lu5, lu6, lu7, lu8;
		double ra1, ra2, ra3, ra4, ra5, ra6, ra7, ra8;

		double sp11, sp12, sp13, sp14, sp15, sp16, sp17, sp18;
		double sp21, sp22, sp23, sp24, sp25, sp26, sp27, sp28;
		double sp31, sp32, sp33, sp34, sp35, sp36, sp37, sp38;
		double sp41, sp42, sp43, sp44, sp45, sp46, sp47, sp48;
		double sp51, sp52, sp53, sp54, sp55, sp56, sp57, sp58;
		double sp61, sp62, sp63, sp64, sp65, sp66, sp67, sp68;
		double sp71, sp72, sp73, sp74, sp75, sp76, sp77, sp78;
		double sp81, sp82, sp83, sp84, sp85, sp86, sp87, sp88;

		double so11, so12, so13, so14, so15, so16, so17, so18;
		double so21, so22, so23, so24, so25, so26, so27, so28;
		double so31, so32, so33, so34, so35, so36, so37, so38;
		double so41, so42, so43, so44, so45, so46, so47, so48;
		double so51, so52, so53, so54, so55, so56, so57, so58;
		double so61, so62, so63, so64, so65, so66, so67, so68;
		double so71, so72, so73, so74, so75, so76, so77, so78;
		double so81, so82, so83, so84, so85, so86, so87, so88;

		RUL    = RhoL*UL;
		RVL    = RhoL*VL;
		RWL    = RhoL*WL;

		double d_1_8PI = 0.039788735772973833942220940843128591;
		double half = 0.5;
		double one = 1.0;
		double zero = 0.0;
		double two = 2.0;
		double four = 4.0;
		double d_SQRT_2 = 0.707106781186547524400844362104849039;
		double d_1_4PI  = 0.079577471545947667884441881686257181;
		double smallB = 1.0e-9;
		double d_PI     = 3.14159265358979323846264338327950288;
		double d_2PI    = 6.28318530717958647692528676655900576;
		double d_4PI    = 12.566370614359172953850573533118;
		int iAveraging = 0;
		int iLaxFriedrix = 0;
		double del2 = inputs.entropy_fix_coeff;
		double hgamma = gamma-1.0;
		BxBL   = d_1_8PI*(BXL*BXL + BYL*BYL + BZL*BZL);
		RUxUL  = half   *(RUL*UL  + RVL*VL  + RWL*WL );



		PL     = PGasL + BxBL;
		EL     = PGasL/hgamma + RUxUL + BxBL;

		AL2    = gamma*PGasL/RhoL;

		RUR    = RhoR*UR;
		RVR    = RhoR*VR;
		RWR    = RhoR*WR;

		BxBR   = d_1_8PI*(BXR*BXR + BYR*BYR + BZR*BZR);
		RUxUR  = half   *(RUR*UR  + RVR*VR  + RWR*WR );

		PR     = PGasR + BxBR;
		ER     = PGasR/hgamma + RUxUR + BxBR;

		AR2    = gamma*PGasR/RhoR;

		if( AL2 > AR2 ) {
			ccmi   = AR2;
			ccma   = AL2;
		} else {
			ccmi   = AL2;
			ccma   = AR2;
		}

		if( iAveraging == 1 ) {
			HL     = (EL + PL)/RhoL;
			HR     = (ER + PR)/RhoR;

			coefRL = one/(one + sqrt( RhoR/RhoL ));
			coefRR = one - coefRL;

			Rho    = RhoR*coefRL + RhoL*coefRR;
			U      = UL  *coefRL + UR  *coefRR;
			V      = VL  *coefRL + VR  *coefRR;
			W      = WL  *coefRL + WR  *coefRR;
			HAV    = HL  *coefRL + HR  *coefRR;

			BX     = BXR *coefRL + BXL *coefRR;
			BY     = BYR *coefRL + BYL *coefRR;
			BZ     = BZR *coefRL + BZL *coefRR;

			BXAV   = half*(BXL  + BXR );

			BX2    = BXAV*BXAV;
			BY2    = BY  *BY;
			BZ2    = BZ  *BZ;

			BYZ2   = BY2 + BZ2;
			BxBH   = d_1_8PI*(BX2 + BYZ2);

			kk     = half*(U*U + V*V + W*W);

			DBX    = BXR - BXL;
			DBY    = BYR - BYL;
			DBZ    = BZR - BZL;

			X      = d_1_8PI*(DBX*DBX + DBY*DBY + DBZ*DBZ)
			         * coefRL*coefRL/RhoL;

			cc     = hgamma*(HAV - kk - d_1_4PI*(BX*BXAV + BYZ2)/Rho)
			         + (two - gamma)*X;
		} else {
			Rho    = half*(RhoL + RhoR);
			U      = half*(UL   + UR  );
			V      = half*(VL   + VR  );
			W      = half*(WL   + WR  );
			BX     = half*(BXL  + BXR );
			BY     = half*(BYL  + BYR );
			BZ     = half*(BZL  + BZR );
			P      = half*(PL   + PR  );

			BX2    = BX*BX;
			BY2    = BY*BY;
			BZ2    = BZ*BZ;

			BYZ2   = BY2 + BZ2;
			BxBH   = d_1_8PI*(BX2 + BYZ2);

			kk     = half*(U*U + V*V + W*W);

			PGas   = P - BxBH;
			cc     = gamma*PGas/Rho;
		}

		bb     = d_1_4PI*BX2/Rho;
		b      = sqrt( bb );

		if( cc < ccmi ) cc    = ccmi;
		if( cc > ccma ) cc    = ccma;
		c      = sqrt( cc );

		if( BYZ2 < smallB*smallB ) {
			hyy    = d_SQRT_2;
			hzz    = d_SQRT_2;

			if( bb > cc ) {
				aaf    = bb;
				aas    = cc;
				af     = b;
				as     = c;
			} else {
				aaf    = cc;
				aas    = bb;
				af     = c;
				as     = b;
			}
		} else {
			hyz    = sqrt( BYZ2 );
			hyy    = BY/hyz;
			hzz    = BZ/hyz;

			szb    = cc + (BxBH + BxBH)/Rho;
			h      = sqrt( szb*szb - four*cc*bb );
			aas    = half*(szb - h);

			if( aas <= zero ) {
				aas    = zero;
				as     = zero;
			} else {
				if( aas >= cc ) {
					aas    = cc;
					as     = c;
				} else {
					as     = sqrt( aas );
				}
			}

			aaf    = half*(szb + h);
			if( aaf <= cc ) {
				aaf    = cc;
				af     = c;
			} else {
				af     = sqrt( aaf );
			}
		}

		if( iLaxFriedrix == 1 ) {
			eigmax   = abs( U ) + af;

			Aeigen1  = eigmax;
			Aeigen2  = eigmax;
			Aeigen3  = eigmax;
			Aeigen4  = eigmax;
			Aeigen5  = eigmax;
			Aeigen6  = eigmax;
			Aeigen7  = eigmax;
			Aeigen8  = eigmax;
		} else {
			eigen1 = U;
			eigen2 = U;
			eigen3 = U + b;
			eigen4 = U - b;
			eigen5 = U + af;
			eigen6 = U - af;
			eigen7 = U + as;
			eigen8 = U - as;

			eigmax = abs( U ) + abs( V ) + abs( W ) + af;

			del    = del2*eigmax;
			dell2  = del*del;

			z      = abs( eigen1 );
			if( z >= del ) {
				Aeigen1  = z;
			} else {
				Aeigen1  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen2 );
			if( z >= del ) {
				Aeigen2  = z;
			} else {
				Aeigen2  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen3 );
			if( z >= del ) {
				Aeigen3  = z;
			} else {
				Aeigen3  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen4 );
			if( z >= del ) {
				Aeigen4  = z;
			} else {
				Aeigen4  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen5 );
			if( z >= del ) {
				Aeigen5  = z;
			} else {
				Aeigen5  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen6 );
			if( z >= del ) {
				Aeigen6  = z;
			} else {
				Aeigen6  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen7 );
			if( z >= del ) {
				Aeigen7  = z;
			} else {
				Aeigen7  = half*(z*z + dell2)/del;
			}

			z      = abs( eigen8 );
			if( z >= del ) {
				Aeigen8  = z;
			} else {
				Aeigen8  = half*(z*z + dell2)/del;
			}
		}

		dfs    = aaf - aas;


		if( dfs < 1.0e-8 ) {
			alf    = one;
			als    = zero;
		} else {
			if( (cc - aas) <= zero ) {
				alf    = zero;
				als    = one;
			} else {
				if( (aaf - cc) <= zero ) {
					alf    = one;
					als    = zero;
				} else {
					alf    = sqrt( (cc  - aas)/dfs );
					als    = sqrt( (aaf - cc )/dfs );
				}
			}
		}



		sih    =-one;
		if( BX >= zero ) sih   = one;

		fl1    =    RUL;
		fl2    = UL*RUL       - BXL*BXL*d_1_4PI + PL;
		fl3    = UL*RVL       - BXL*BYL*d_1_4PI;
		fl4    = UL*RWL       - BXL*BZL*d_1_4PI;
		fl5    = UL*(EL + PL) - BXL*(UL*BXL + VL*BYL + WL*BZL)*d_1_4PI;
		fl6    = zero;
		fl7    = UL*BYL       - BXL*VL;
		fl8    = UL*BZL       - BXL*WL;

		fr1    =    RUR;
		fr2    = UR*RUR       - BXR*BXR*d_1_4PI + PR;
		fr3    = UR*RVR       - BXR*BYR*d_1_4PI;
		fr4    = UR*RWR       - BXR*BZR*d_1_4PI;
		fr5    = UR*(ER + PR) - BXR*(UR*BXR + VR*BYR + WR*BZR)*d_1_4PI;
		fr6    = zero;
		fr7    = UR*BYR       - BXR*VR;
		fr8    = UR*BZR       - BXR*WR;

		du1    = RhoL - RhoR;
		du2    = RUL  - RUR;
		du3    = RVL  - RVR;
		du4    = RWL  - RWR;
		du5    = EL   - ER;
		du6    = BXL  - BXR;
		du7    = BYL  - BYR;
		du8    = BZL  - BZR;

		du5    =-hgamma*(-kk*du1 +   U*du2 +  V*du3 +  W*du4 - du5
		                 + (BX*du6 + BY*du7 + BZ*du8)*d_1_4PI);

		du2    = (-U*du1 + du2)/Rho;
		du3    = (-V*du1 + du3)/Rho;
		du4    = (-W*du1 + du4)/Rho;

		sqPiRho  = sqrt( d_PI *Rho );
		sq2PiRho = sqrt( d_2PI*Rho );

		sp11   = one;
		sp12   = zero;
		sp13   = zero;
		sp14   = zero;
		sp15   = Rho*alf;
		sp16   = Rho*alf;
		sp17   = Rho*als;
		sp18   = Rho*als;

		sp21   = zero;
		sp22   = zero;
		sp23   = zero;
		sp24   = zero;
		sp25   = alf*af;
		sp26   =-alf*af;
		sp27   = als*as;
		sp28   =-als*as;

		sp31   = zero;
		sp32   = zero;
		sp33   =-hzz*d_SQRT_2;
		sp34   =-hzz*d_SQRT_2;
		sp35   =-als*as*hyy*sih;
		sp36   = als*as*hyy*sih;
		sp37   = alf*af*hyy*sih;
		sp38   =-alf*af*hyy*sih;

		sp41   = zero;
		sp42   = zero;
		sp43   = hyy*d_SQRT_2;
		sp44   = hyy*d_SQRT_2;
		sp45   =-als*as*hzz*sih;
		sp46   = als*as*hzz*sih;
		sp47   = alf*af*hzz*sih;
		sp48   =-alf*af*hzz*sih;

		sp51   = zero;
		sp52   = zero;
		sp53   = zero;
		sp54   = zero;
		sp55   = alf*Rho*cc;
		sp56   = alf*Rho*cc;
		sp57   = als*Rho*cc;
		sp58   = als*Rho*cc;

		sp61   = zero;
		sp62   = one;
		sp63   = zero;
		sp64   = zero;
		sp65   = zero;
		sp66   = zero;
		sp67   = zero;
		sp68   = zero;

		sp71   = zero;
		sp72   = zero;
		sp73   = hzz*sq2PiRho*sih;
		sp74   =-hzz*sq2PiRho*sih;
		sp75   = two*als*sqPiRho*c*hyy;
		sp76   = two*als*sqPiRho*c*hyy;
		sp77   =-two*alf*sqPiRho*c*hyy;
		sp78   =-two*alf*sqPiRho*c*hyy;

		sp81   = zero;
		sp82   = zero;
		sp83   =-hyy*sq2PiRho*sih;
		sp84   = hyy*sq2PiRho*sih;
		sp85   = two*als*sqPiRho*c*hzz;
		sp86   = two*als*sqPiRho*c*hzz;
		sp87   =-two*alf*sqPiRho*c*hzz;
		sp88   =-two*alf*sqPiRho*c*hzz;

		so11   = one;
		so12   = zero;
		so13   = zero;
		so14   = zero;
		so15   =-one/cc;
		so16   = zero;
		so17   = zero;
		so18   = zero;

		so21   = zero;
		so22   = zero;
		so23   = zero;
		so24   = zero;
		so25   = zero;
		so26   = one;
		so27   = zero;
		so28   = zero;

		so31   = zero;
		so32   = zero;
		so33   =-hzz*d_SQRT_2;
		so34   = hyy*d_SQRT_2;
		so35   = zero;
		so36   = zero;
		so37   = half*hzz*sih/sq2PiRho;
		so38   =-half*hyy*sih/sq2PiRho;

		so41   = zero;
		so42   = zero;
		so43   =-hzz*d_SQRT_2;
		so44   = hyy*d_SQRT_2;
		so45   = zero;
		so46   = zero;
		so47   =-half*hzz*sih/sq2PiRho;
		so48   = half*hyy*sih/sq2PiRho;

		so51   = zero;
		so52   = half*alf*af/cc;
		so53   =-half*als*as*hyy*sih/cc;
		so54   =-half*als*as*hzz*sih/cc;
		so55   = half*alf/(cc*Rho);
		so56   = zero;
		so57   = 0.25*als*hyy/(c*sqPiRho);
		so58   = 0.25*als*hzz/(c*sqPiRho);

		so61   = zero;
		so62   =-half*alf*af/cc;
		so63   = half*als*as*hyy*sih/cc;
		so64   = half*als*as*hzz*sih/cc;
		so65   = half*alf/(cc*Rho);
		so66   = zero;
		so67   = 0.25*als*hyy/(c*sqPiRho);
		so68   = 0.25*als*hzz/(c*sqPiRho);

		so71   = zero;
		so72   = half*als*as/cc;
		so73   = half*alf*af*hyy*sih/cc;
		so74   = half*alf*af*hzz*sih/cc;
		so75   = half*als/(Rho*cc);
		so76   = zero;
		so77   =-0.25*alf*hyy/(c*sqPiRho);
		so78   =-0.25*alf*hzz/(c*sqPiRho);

		so81   = zero;
		so82   =-half*als*as/cc;
		so83   =-half*alf*af*hyy*sih/cc;
		so84   =-half*alf*af*hzz*sih/cc;
		so85   = half*als/(Rho*cc);
		so86   = zero;
		so87   =-0.25*alf*hyy/(c*sqPiRho);
		so88   =-0.25*alf*hzz/(c*sqPiRho);

		lu1    = Aeigen1*(so11*du1 + so12*du2 + so13*du3 + so14*du4
		                  + so15*du5 + so16*du6 + so17*du7 + so18*du8);
		lu2    = Aeigen2*(so21*du1 + so22*du2 + so23*du3 + so24*du4
		                  + so25*du5 + so26*du6 + so27*du7 + so28*du8);
		lu3    = Aeigen3*(so31*du1 + so32*du2 + so33*du3 + so34*du4
		                  + so35*du5 + so36*du6 + so37*du7 + so38*du8);
		lu4    = Aeigen4*(so41*du1 + so42*du2 + so43*du3 + so44*du4
		                  + so45*du5 + so46*du6 + so47*du7 + so48*du8);
		lu5    = Aeigen5*(so51*du1 + so52*du2 + so53*du3 + so54*du4
		                  + so55*du5 + so56*du6 + so57*du7 + so58*du8);
		lu6    = Aeigen6*(so61*du1 + so62*du2 + so63*du3 + so64*du4
		                  + so65*du5 + so66*du6 + so67*du7 + so68*du8);
		lu7    = Aeigen7*(so71*du1 + so72*du2 + so73*du3 + so74*du4
		                  + so75*du5 + so76*du6 + so77*du7 + so78*du8);
		lu8    = Aeigen8*(so81*du1 + so82*du2 + so83*du3 + so84*du4
		                  + so85*du5 + so86*du6 + so87*du7 + so88*du8);

		ra1    = sp11*lu1 + sp12*lu2 + sp13*lu3 + sp14*lu4
		         + sp15*lu5 + sp16*lu6 + sp17*lu7 + sp18*lu8;
		ra2    = sp21*lu1 + sp22*lu2 + sp23*lu3 + sp24*lu4
		         + sp25*lu5 + sp26*lu6 + sp27*lu7 + sp28*lu8;
		ra3    = sp31*lu1 + sp32*lu2 + sp33*lu3 + sp34*lu4
		         + sp35*lu5 + sp36*lu6 + sp37*lu7 + sp38*lu8;
		ra4    = sp41*lu1 + sp42*lu2 + sp43*lu3 + sp44*lu4
		         + sp45*lu5 + sp46*lu6 + sp47*lu7 + sp48*lu8;
		ra5    = sp51*lu1 + sp52*lu2 + sp53*lu3 + sp54*lu4
		         + sp55*lu5 + sp56*lu6 + sp57*lu7 + sp58*lu8;
		ra6    = sp61*lu1 + sp62*lu2 + sp63*lu3 + sp64*lu4
		         + sp65*lu5 + sp66*lu6 + sp67*lu7 + sp68*lu8;
		ra7    = sp71*lu1 + sp72*lu2 + sp73*lu3 + sp74*lu4
		         + sp75*lu5 + sp76*lu6 + sp77*lu7 + sp78*lu8;
		ra8    = sp81*lu1 + sp82*lu2 + sp83*lu3 + sp84*lu4
		         + sp85*lu5 + sp86*lu6 + sp87*lu7 + sp88*lu8;

		ra5    = ra1*kk     + Rho    *(U *ra2 + V *ra3 + W *ra4)
		         + ra5/hgamma + d_1_4PI*(BX*ra6 + BY*ra7 + BZ*ra8);
		ra2    = U*ra1 + Rho*ra2;
		ra3    = V*ra1 + Rho*ra3;
		ra4    = W*ra1 + Rho*ra4;

		// ra1 = zero;
		// ra2 = zero;
		// ra3 = zero;
		// ra4 = zero;
		// ra5 = zero;
		// ra6 = zero;
		// ra7 = zero;
		// ra8 = zero;


		if (a_dir == 0) {
			a_out(0)  = half*(fl1 + fr1 + ra1);
			a_out(1)  = half*(fl2 + fr2 + ra2);
			a_out(2)  = half*(fl3 + fr3 + ra3);
			a_out(3)  = half*(fl4 + fr4 + ra4);
			a_out(4)  = half*(fl5 + fr5 + ra5);
			a_out(5)  = half*(fl6 + fr6 + ra6);
			a_out(6)  = half*(fl7 + fr7 + ra7);
			a_out(7)  = half*(fl8 + fr8 + ra8);
			#if TURB == 1
			a_out(iZ2) = 0.;
			a_out(iSIGMA) = 0.;
			a_out(iLAMBDA) = 0.;
			#endif
		}
		if (a_dir == 1) {
			a_out(0)  = half*(fl1 + fr1 + ra1);
			a_out(2)  = half*(fl2 + fr2 + ra2);
			a_out(3)  = half*(fl3 + fr3 + ra3);
			a_out(1)  = half*(fl4 + fr4 + ra4);
			a_out(4)  = half*(fl5 + fr5 + ra5);
			a_out(6)  = half*(fl6 + fr6 + ra6);
			a_out(7)  = half*(fl7 + fr7 + ra7);
			a_out(5)  = half*(fl8 + fr8 + ra8);
			#if TURB == 1
			a_out(iZ2) = 0.;
			a_out(iSIGMA) = 0.;
			a_out(iLAMBDA) = 0.;
			#endif
		}
		if (a_dir == 2) {
			a_out(0)  = half*(fl1 + fr1 + ra1);
			a_out(3)  = half*(fl2 + fr2 + ra2);
			a_out(1)  = half*(fl3 + fr3 + ra3);
			a_out(2)  = half*(fl4 + fr4 + ra4);
			a_out(4)  = half*(fl5 + fr5 + ra5);
			a_out(7)  = half*(fl6 + fr6 + ra6);
			a_out(5)  = half*(fl7 + fr7 + ra7);
			a_out(6)  = half*(fl8 + fr8 + ra8);
			#if TURB == 1
			a_out(iZ2) = 0.;
			a_out(iSIGMA) = 0.;
			a_out(iLAMBDA) = 0.;
			#endif
		}


	}
	PROTO_KERNEL_END(roe8waveStateF, roe8waveState)


	PROTO_KERNEL_START
	void lambdacalcF(Var<double,1>& a_lambda,
	                 const State& a_W_low,
	                 const State& a_W_high,
	                 int a_d,
	                 double a_gamma)
	{
		double gamma = a_gamma;
		double rho=0., u=0., v=0., w=0., p=0., Bx=0., By=0., Bz=0., ce, af, B_mag, u_mag, Bdir, udir;

		rho = 0.5 * (a_W_low(0) + a_W_high(0));
		u   = 0.5 * (a_W_low(1) + a_W_high(1));
		v   = 0.5 * (a_W_low(2) + a_W_high(2));
		w   = 0.5 * (a_W_low(3) + a_W_high(3));
		p   = 0.5 * (a_W_low(4) + a_W_high(4));
		Bx  = 0.5 * (a_W_low(5) + a_W_high(5));
		By  = 0.5 * (a_W_low(6) + a_W_high(6));
		Bz  = 0.5 * (a_W_low(7) + a_W_high(7));

		if (a_d == 0) {
			Bdir = Bx;
			udir = u;
		};
		if (a_d == 1) {
			Bdir = By;
			udir = v;
		};
		if (a_d == 2) {
			Bdir = Bz;
			udir = w;
		};

		if (p < 0.0) p = 0.0;
		ce = sqrt(gamma*p/rho);
		B_mag = sqrt(Bx*Bx+By*By+Bz*Bz);
		af = sqrt(ce*ce + B_mag*B_mag/4.0/c_PI/rho);
		// af = 0.5*(sqrt((ce*ce)+( B_mag*B_mag/(4.0*c_PI*rho) )+( abs(Bdir)*ce/sqrt(c_PI*rho) ))+sqrt((ce*ce)+( B_mag*B_mag/(4.0*c_PI*rho) )-( abs(Bdir)*ce/sqrt(c_PI*rho) )));
		a_lambda(0) = af + abs(udir);
	}
	PROTO_KERNEL_END(lambdacalcF, lambdacalc)

	PROTO_KERNEL_START
	void fastMSspeedcalcF(Var<double,1>& a_fastMSspeed,
	                 const State& a_W_low,
	                 const State& a_W_high,
	                 const Var<double,DIM>& a_A_mag_face,
	                 int a_d,
	                 double a_gamma)
	{
		double gamma = a_gamma;
		double rho=0., u=0., v=0., w=0., p=0., Bx=0., By=0., Bz=0., ce, af, B_mag, u_mag, Bdir, udir;

		rho = 0.5 * (a_W_low(0) + a_W_high(0));
		u   = 0.5 * (a_W_low(1) + a_W_high(1));
		v   = 0.5 * (a_W_low(2) + a_W_high(2));
		w   = 0.5 * (a_W_low(3) + a_W_high(3));
		p   = 0.5 * (a_W_low(4) + a_W_high(4));
		Bx  = 0.5 * (a_W_low(5) + a_W_high(5));
		By  = 0.5 * (a_W_low(6) + a_W_high(6));
		Bz  = 0.5 * (a_W_low(7) + a_W_high(7));

		if (a_d == 0) {
			Bdir = Bx;
			udir = u;
		};
		if (a_d == 1) {
			Bdir = By;
			udir = v;
		};
		if (a_d == 2) {
			Bdir = Bz;
			udir = w;
		};

		if (p < 0.0) p = 0.0;
		ce = sqrt(gamma*p/rho);
		B_mag = sqrt(Bx*Bx+By*By+Bz*Bz);
		af = sqrt(ce*ce + B_mag*B_mag/4.0/c_PI/rho);
		a_fastMSspeed(0) = af/a_A_mag_face(a_d);
	}
	PROTO_KERNEL_END(fastMSspeedcalcF, fastMSspeedcalc)

	PROTO_KERNEL_START
	void getFluxF(State& a_F,
	              const State& a_W,
	              int a_d,
	              double a_gamma)
	{
		double rho, p0, e, v2, B2, vB;
		double gamma = a_gamma;
		double mult1=0.;
		double mult2=0.;
		double mult3=0.;
		if (a_d == 0) {mult1=1.0;}
		if (a_d == 1) {mult2=1.0;}
		if (a_d == 2) {mult3=1.0;}
		rho = a_W(0);

		v2 = a_W(1)*a_W(1) + a_W(2)*a_W(2) + a_W(3)*a_W(3);
		B2 = a_W(5)*a_W(5) + a_W(6)*a_W(6) + a_W(7)*a_W(7);
		vB = a_W(1)*a_W(5) + a_W(2)*a_W(6) + a_W(3)*a_W(7);
		p0 = a_W(4) + B2/8.0/c_PI;
		e  = a_W(4)/(gamma-1.0) + rho*v2/2.0 + B2/8.0/c_PI;
		a_F(0) = rho*a_W(1+a_d);
		a_F(1) = rho*a_W(1)*a_W(1+a_d) + mult1*p0 - a_W(5)*a_W(5+a_d)/4.0/c_PI;
		a_F(2) = rho*a_W(2)*a_W(1+a_d) + mult2*p0 - a_W(6)*a_W(5+a_d)/4.0/c_PI;
		a_F(3) = rho*a_W(3)*a_W(1+a_d) + mult3*p0 - a_W(7)*a_W(5+a_d)/4.0/c_PI;
		a_F(4) = (e+p0)*a_W(1+a_d) - a_W(5+a_d)*vB/4.0/c_PI;
		a_F(5) = (mult2+mult3)*(a_W(1+a_d)*a_W(5) - a_W(1)*a_W(5+a_d));
		a_F(6) = (mult1+mult3)*(a_W(1+a_d)*a_W(6) - a_W(2)*a_W(5+a_d));
		a_F(7) = (mult1+mult2)*(a_W(1+a_d)*a_W(7) - a_W(3)*a_W(5+a_d));

	}
	PROTO_KERNEL_END(getFluxF, getFlux)

	PROTO_KERNEL_START
	void rusanovStateF(State& a_out,
	                   const State& a_W_lo,
	                   const State& a_W_hi,
	                   const State& a_F_lo,
	                   const State& a_F_hi,
	                   const Var<double,1>& a_lambda,
	                   int a_dir,
	                   double a_gamma)
	{
		double gamma = a_gamma;

		double rho_lo, rhou_lo, rhov_lo, rhow_lo, e_lo, p_lo, Bx_lo, By_lo, Bz_lo, B2_lo, v2_lo;
		double rho_hi, rhou_hi, rhov_hi, rhow_hi, e_hi, p_hi, Bx_hi, By_hi, Bz_hi, B2_hi, v2_hi;
		rho_lo  = a_W_lo(0);
		rho_hi  = a_W_hi(0);
		rhou_lo = rho_lo*a_W_lo(1);
		rhou_hi = rho_hi*a_W_hi(1);
		rhov_lo = rho_lo*a_W_lo(2);
		rhov_hi = rho_hi*a_W_hi(2);
		rhow_lo = rho_lo*a_W_lo(3);
		rhow_hi = rho_hi*a_W_hi(3);
		p_lo    = a_W_lo(4);
		p_hi    = a_W_hi(4);
		Bx_lo   = a_W_lo(5);
		Bx_hi   = a_W_hi(5);
		By_lo   = a_W_lo(6);
		By_hi   = a_W_hi(6);
		Bz_lo   = a_W_lo(7);
		Bz_hi   = a_W_hi(7);
		B2_lo   = Bx_lo*Bx_lo + By_lo*By_lo + Bz_lo*Bz_lo;
		B2_hi   = Bx_hi*Bx_hi + By_hi*By_hi + Bz_hi*Bz_hi;
		v2_lo   = a_W_lo(1)*a_W_lo(1) + a_W_lo(2)*a_W_lo(2) + a_W_lo(3)*a_W_lo(3);
		v2_hi   = a_W_hi(1)*a_W_hi(1) + a_W_hi(2)*a_W_hi(2) + a_W_hi(3)*a_W_hi(3);
		e_lo    = p_lo/(gamma-1.0) + rho_lo*v2_lo/2.0 + B2_lo/8.0/c_PI;
		e_hi    = p_hi/(gamma-1.0) + rho_hi*v2_hi/2.0 + B2_hi/8.0/c_PI;
		a_out(0) = 0.5*(a_F_hi(0) + a_F_lo(0) - abs(a_lambda(0))*(rho_hi-rho_lo));
		a_out(1) = 0.5*(a_F_hi(1) + a_F_lo(1) - abs(a_lambda(0))*(rhou_hi-rhou_lo));
		a_out(2) = 0.5*(a_F_hi(2) + a_F_lo(2) - abs(a_lambda(0))*(rhov_hi-rhov_lo));
		a_out(3) = 0.5*(a_F_hi(3) + a_F_lo(3) - abs(a_lambda(0))*(rhow_hi-rhow_lo));
		a_out(4) = 0.5*(a_F_hi(4) + a_F_lo(4) - abs(a_lambda(0))*(e_hi-e_lo));
		a_out(5) = 0.5*(a_F_hi(5) + a_F_lo(5) - abs(a_lambda(0))*(Bx_hi-Bx_lo));
		a_out(6) = 0.5*(a_F_hi(6) + a_F_lo(6) - abs(a_lambda(0))*(By_hi-By_lo));
		a_out(7) = 0.5*(a_F_hi(7) + a_F_lo(7) - abs(a_lambda(0))*(Bz_hi-Bz_lo));
		#if TURB == 1
		a_out(iZ2) = 0.;
		a_out(iSIGMA) = 0.;
		a_out(iLAMBDA) = 0.;
		#endif

	}
	PROTO_KERNEL_END(rusanovStateF, rusanovState)


	void Rusanov_Solver(BoxData<double,NUMCOMPS>& a_F_f,
	                    const BoxData<double,NUMCOMPS>& a_W_low,
	                    const BoxData<double,NUMCOMPS>& a_W_high,
	                    const int a_d,
	                    const double a_gamma)
	{
		Scalar Lambda_f = forall<double>(lambdacalc, a_W_low, a_W_high, a_d, a_gamma);
		Vector F_low = forall<double,NUMCOMPS>(getFlux, a_W_low, a_d, a_gamma);
		Vector F_high = forall<double,NUMCOMPS>(getFlux, a_W_high, a_d, a_gamma);
		a_F_f = forall<double,NUMCOMPS>(rusanovState, a_W_low, a_W_high, F_low, F_high, Lambda_f, a_d, a_gamma);
	}

	void Roe8Wave_Solver(BoxData<double,NUMCOMPS>& a_F_f,
	                     const BoxData<double,NUMCOMPS>& a_W_low,
	                     const BoxData<double,NUMCOMPS>& a_W_high,
	                     const int a_d,
	                     const double a_gamma)
	{
		a_F_f = forall<double,NUMCOMPS>(roe8waveState,a_W_low,a_W_high,a_d,a_gamma);
	}
}
