namespace NetworkPoolLib
{
    public class Password
    {
        public string Pass { get; private set; }

        public Password(string pass)
        {
            Pass = pass;
        }

        public static bool CheckValid()
        {
            bool result = false;
            return result;
        }
    }
}
