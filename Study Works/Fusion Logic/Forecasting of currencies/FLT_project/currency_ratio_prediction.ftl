PROJECT {
  NAME       = currency_ratio_prediction.ftl;
  TITLE      =currency_ratio_prediction;
  DATEFORMAT = YYYY.DD.MM;
  LASTCHANGE = 2016.12.05;
  CREATED    = 2016.12.05;
  SHELL      = ON_LINE;
  SHELLOPTIONS {
    ONLINE_REFRESHTIME  = 55;
    ONLINE_TIMEOUTCOUNT = 1100;
    ONLINE_CODE         = OFF;
    ONLINE_TRACE_BUFFER = (OFF, PAR(0));
    COMMENTS            = ON;
    FTL_BUFFER          = (OFF, PAR(1));
    PASSWORD            = OFF;
    PUBLIC_IO           = ON;
    FAST_CMBF           = OFF;
    FAST_COA            = ON;
    BTYPE               = DOUBLE;
    C_TYPE              = ANSI;
  } /* SHELLOPTIONS */
  MODEL {
    VARIABLE_SECTION {
      LVAR {
        NAME       = Var1;
        BASEVAR    = Units;
        LVRANGE    = MIN(0.6854), MAX(0.7817),
                     MINDEF(0), MAXDEF(65535),
                     DEFAULT_OUTPUT(0.73355);
        RESOLUTION = XGRID(0.00963), YGRID(0.1),
                     SHOWGRID (ON), SNAPTOGRID(OFF);
        COLOR      = RED (255), GREEN (0), BLUE (0);
        INPUT      = CMBF;
        POS        = -198, -196;
        TERM {
          TERMNAME = term1;
          POINTS   = (0.6854, 1.0),
                     (0.7068026171875, 1.0),
                     (0.7340156796875, 0.0),
                     (0.7817, 0.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (255), GREEN (0), BLUE (0);
        }
        TERM {
          TERMNAME = term2;
          POINTS   = (0.6854, 0.0),
                     (0.7068026171875, 0.0),
                     (0.7340156796875, 1.0),
                     (0.7560754609375, 0.0),
                     (0.7817, 0.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (128), BLUE (0);
        }
        TERM {
          TERMNAME = term3;
          POINTS   = (0.6854, 0.0),
                     (0.7340156796875, 0.0),
                     (0.7560754609375, 1.0),
                     (0.7817, 1.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (0), BLUE (255);
        }
      }  /* LVAR */
      LVAR {
        NAME       = Var2;
        BASEVAR    = Units;
        LVRANGE    = MIN(0.6854), MAX(0.7817),
                     MINDEF(0), MAXDEF(65535),
                     DEFAULT_OUTPUT(0.73355);
        RESOLUTION = XGRID(0.00963), YGRID(0.1),
                     SHOWGRID (ON), SNAPTOGRID(OFF);
        COLOR      = RED (0), GREEN (128), BLUE (0);
        INPUT      = CMBF;
        POS        = -198, -56;
        TERM {
          TERMNAME = term1;
          POINTS   = (0.6854, 1.0),
                     (0.709474, 1.0),
                     (0.73355, 0.0),
                     (0.7817, 0.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (255), GREEN (0), BLUE (0);
        }
        TERM {
          TERMNAME = term2;
          POINTS   = (0.6854, 0.0),
                     (0.709474, 0.0),
                     (0.73355, 1.0),
                     (0.754798, 0.0),
                     (0.7817, 0.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (128), BLUE (0);
        }
        TERM {
          TERMNAME = term3;
          POINTS   = (0.6854, 0.0),
                     (0.73355, 0.0),
                     (0.754798, 1.0),
                     (0.7817, 1.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (0), BLUE (255);
        }
      }  /* LVAR */
      LVAR {
        NAME       = Var3;
        BASEVAR    = Units;
        LVRANGE    = MIN(0.6854), MAX(0.7817),
                     MINDEF(0), MAXDEF(65535),
                     DEFAULT_OUTPUT(0.73355);
        RESOLUTION = XGRID(0.00963), YGRID(0.1),
                     SHOWGRID (ON), SNAPTOGRID(OFF);
        COLOR      = RED (0), GREEN (0), BLUE (255);
        INPUT      = CMBF;
        POS        = -198, 85;
        TERM {
          TERMNAME = term1;
          POINTS   = (0.6854, 1.0),
                     (0.711504, 1.0),
                     (0.73218, 0.0),
                     (0.7817, 0.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (255), GREEN (0), BLUE (0);
        }
        TERM {
          TERMNAME = term2;
          POINTS   = (0.6854, 0.0),
                     (0.711504, 0.0),
                     (0.73218, 1.0),
                     (0.756852, 0.0),
                     (0.7817, 0.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (128), BLUE (0);
        }
        TERM {
          TERMNAME = term3;
          POINTS   = (0.6854, 0.0),
                     (0.73218, 0.0),
                     (0.756852, 1.0),
                     (0.7817, 1.0) : OPEN (0.6854, 0.7817);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (0), BLUE (255);
        }
      }  /* LVAR */
      LVAR {
        NAME       = Var4;
        BASEVAR    = Units;
        LVRANGE    = MIN(0.661325), MAX(0.805775),
                     MINDEF(0), MAXDEF(65535),
                     DEFAULT_OUTPUT(0.73355);
        RESOLUTION = XGRID(0.014445), YGRID(0.1),
                     SHOWGRID (ON), SNAPTOGRID(OFF);
        COLOR      = RED (128), GREEN (0), BLUE (0);
        OUTPUT     = COM;
        POS        = 463, -55;
        TERM {
          TERMNAME = term1;
          POINTS   = (0.661325, 0.0),
                     (0.67574166015625, 1.0),
                     (0.709475, 0.0),
                     (0.805775, 0.0) : OPEN (0.661325, 0.805775);
          SHAPE    = LINEAR;
          COLOR    = RED (255), GREEN (0), BLUE (0);
        }
        TERM {
          TERMNAME = term2;
          POINTS   = (0.661325, 0.0),
                     (0.67574166015625, 0.0),
                     (0.709475, 1.0),
                     (0.73355, 0.0),
                     (0.805775, 0.0) : OPEN (0.661325, 0.805775);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (128), BLUE (0);
        }
        TERM {
          TERMNAME = term3;
          POINTS   = (0.661325, 0.0),
                     (0.709475, 0.0),
                     (0.73355, 1.0),
                     (0.761485, 0.0),
                     (0.805775, 0.0) : OPEN (0.661325, 0.805775);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (0), BLUE (255);
        }
        TERM {
          TERMNAME = term4;
          POINTS   = (0.661325, 0.0),
                     (0.73355, 0.0),
                     (0.761485, 1.0),
                     (0.7953896015625, 0.0),
                     (0.805775, 0.0) : OPEN (0.661325, 0.805775);
          SHAPE    = LINEAR;
          COLOR    = RED (128), GREEN (0), BLUE (0);
        }
        TERM {
          TERMNAME = term5;
          POINTS   = (0.661325, 0.0),
                     (0.761485, 0.0),
                     (0.7953896015625, 1.0),
                     (0.805775, 0.0) : OPEN (0.661325, 0.805775);
          SHAPE    = LINEAR;
          COLOR    = RED (0), GREEN (128), BLUE (128);
        }
      }  /* LVAR */
    }  /* VARIABLE_SECTION */

    OBJECT_SECTION {
      RULEBLOCK {
        NAME        = RB1;
        INPUT       = Var1, Var2, Var3;
        OUTPUT      = Var4;
        AGGREGATION = (MIN_MAX, PAR (0.0));
        RESULT_AGGR = MAX;
        POS         = 120, -68;
        RULES {
          IF    Var1 = term1
            AND Var2 = term1
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.820  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.359  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.000  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.023  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term1
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.438  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term1
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term2
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term2
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term2
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term3
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term3
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term1
            AND Var2 = term3
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term1
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.336  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.352  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term1
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term1
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term2
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term2
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term2
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term3
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term3
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term2
            AND Var2 = term3
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.367  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.422  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term1
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term1
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term1
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term2
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term2
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.672  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term2
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term3
            AND Var3 = term1
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term3
            AND Var3 = term2
          THEN  Var4 = term1   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.430  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.500  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.594  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 0.500  :  OPEN (0.00, 1.00);
          IF    Var1 = term3
            AND Var2 = term3
            AND Var3 = term3
          THEN  Var4 = term1   WITH 0.297  :  OPEN (0.00, 1.00)
            AND Var4 = term2   WITH 0.172  :  OPEN (0.00, 1.00)
            AND Var4 = term3   WITH 0.000  :  OPEN (0.00, 1.00)
            AND Var4 = term4   WITH 0.594  :  OPEN (0.00, 1.00)
            AND Var4 = term5   WITH 1.000  :  OPEN (0.00, 1.00);
        }  /* RULES */
      }  /* RULEBLOCK */
    }  /* OBJECT_SECTION */
  }  /* MODEL */
}  /* PROJECT */ 
/* fuzzyTECH 5.54d Professional Edition */
Ûý«ŠÕçwê
  ©Àî¢£þò¶© = 20160512032058UT;
}  /* ONLINE */

NEUROFUZZY {
  LEARNRULE     =RandomMethod;
  STEPWIDTHDOS  = 0.100000;
  STEPWIDTHTERM = 1.000000;
  MAXDEVIATION  = (8.898926, 1.000000, 0.750000);
  AVGDEVIATION  = 0.100000;
  MAXSTEPS      = 100;
  NEURONS       = 1;
  DATASEQUENCE  = RANDOM;
  UPDATEDBGWIN  = OFF;
}  /* NEUROFUZZY */